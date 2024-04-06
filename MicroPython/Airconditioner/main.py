import urequests as request
import ujson
import uasyncio as asyncio
from machine import I2C, Pin, reset, WDT
import math
try:
    import utime as time
except ImportError:
    import time
import onewire, ds18x20
MPU6050_ADDR = 0x68

MPU6050_GYRO_CONFIG = 0x1B
MPU6050_ACCEL_CONFIG = 0x1C
MPU6050_ACCEL_CONFIG2 = 0x1D
MPU6050_ACCEL_XOUT_H = 0x3B
MPU6050_ACCEL_XOUT_L = 0x3C
MPU6050_ACCEL_YOUT_H = 0x3D
MPU6050_ACCEL_YOUT_L = 0x3E
MPU6050_ACCEL_ZOUT_H = 0x3F
MPU6050_ACCEL_ZOUT_L = 0x40
MPU6050_GYRO_XOUT_H = 0x43
MPU6050_GYRO_XOUT_L = 0x44
MPU6050_GYRO_YOUT_H = 0x45
MPU6050_GYRO_YOUT_L = 0x46
MPU6050_GYRO_ZOUT_H = 0x47
MPU6050_GYRO_ZOUT_L = 0x48
MPU6050_PWR_MGMT_1 = 0x6B
MPU6050_WHO_AM_I = 0x75

MPU6050_LSBG = 16384.0
MPU6050_LSBDS = 131.0

is_micropython = hasattr(time, 'ticks_diff')


class DeltaT():
    def __init__(self, timediff):
        if timediff is None:
            self.expect_ts = False
            if is_micropython:
                self.timediff = lambda start, end : time.ticks_diff(start, end)/1000000
            else:
                raise ValueError('You must define a timediff function')
        else:
            self.expect_ts = True
            self.timediff = timediff
        self.start_time = None

    def __call__(self, ts):
        if self.expect_ts:
            if ts is None:
                raise ValueError('Timestamp expected but not supplied.')
        else:
            if is_micropython:
                ts = time.ticks_us()
            else:
                raise RuntimeError('Not MicroPython: provide timestamps and a timediff function')
        # ts is now valid
        if self.start_time is None:  # 1st call: self.start_time is invalid
            self.start_time = ts
            return 0.0001  # 100μs notional delay. 1st reading is invalid in any case

        dt = self.timediff(ts, self.start_time)
        self.start_time = ts
        return dt


class Fusion(object):
    declination = 0

    def __init__(self, timediff=None):
        self.magbias = (0, 0, 0)            # local magnetic bias factors: set from calibration
        self.deltat = DeltaT(timediff)      # Time between updates
        self.q = [1.0, 0.0, 0.0, 0.0]       # vector to hold quaternion
        GyroMeasError = math.radians(40)         # Original code indicates this leads to a 2 sec response time
        self.beta = math.sqrt(3.0 / 4.0) * GyroMeasError  # compute beta (see README)
        self.pitch = 0
        self.heading = 0
        self.roll = 0

    def update_nomag(self, accel, gyro, ts=None):    # 3-tuples (x, y, z) for accel, gyro
        ax, ay, az = accel                  # Units G (but later normalised)
        gx, gy, gz = (math.radians(x) for x in gyro) # Units deg/s
        q1, q2, q3, q4 = (self.q[x] for x in range(4))   # short name local variable for readability
        # Auxiliary variables to avoid repeated arithmetic
        _2q1 = 2 * q1
        _2q2 = 2 * q2
        _2q3 = 2 * q3
        _2q4 = 2 * q4
        _4q1 = 4 * q1
        _4q2 = 4 * q2
        _4q3 = 4 * q3
        _8q2 = 8 * q2
        _8q3 = 8 * q3
        q1q1 = q1 * q1
        q2q2 = q2 * q2
        q3q3 = q3 * q3
        q4q4 = q4 * q4

        # Normalise accelerometer measurement
        norm = math.sqrt(ax * ax + ay * ay + az * az)
        if (norm == 0):
            return # handle NaN
        norm = 1 / norm        # use reciprocal for division
        ax *= norm
        ay *= norm
        az *= norm

        # Gradient decent algorithm corrective step
        s1 = _4q1 * q3q3 + _2q3 * ax + _4q1 * q2q2 - _2q2 * ay
        s2 = _4q2 * q4q4 - _2q4 * ax + 4 * q1q1 * q2 - _2q1 * ay - _4q2 + _8q2 * q2q2 + _8q2 * q3q3 + _4q2 * az
        s3 = 4 * q1q1 * q3 + _2q1 * ax + _4q3 * q4q4 - _2q4 * ay - _4q3 + _8q3 * q2q2 + _8q3 * q3q3 + _4q3 * az
        s4 = 4 * q2q2 * q4 - _2q2 * ax + 4 * q3q3 * q4 - _2q3 * ay
        norm = 1 / math.sqrt(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4)    # normalise step magnitude
        s1 *= norm
        s2 *= norm
        s3 *= norm
        s4 *= norm

        # Compute rate of change of quaternion
        qDot1 = 0.5 * (-q2 * gx - q3 * gy - q4 * gz) - self.beta * s1
        qDot2 = 0.5 * (q1 * gx + q3 * gz - q4 * gy) - self.beta * s2
        qDot3 = 0.5 * (q1 * gy - q2 * gz + q4 * gx) - self.beta * s3
        qDot4 = 0.5 * (q1 * gz + q2 * gy - q3 * gx) - self.beta * s4

        # Integrate to yield quaternion
        deltat = self.deltat(ts)
        q1 += qDot1 * deltat
        q2 += qDot2 * deltat
        q3 += qDot3 * deltat
        q4 += qDot4 * deltat
        norm = 1 / math.sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4)    # normalise quaternion
        self.q = q1 * norm, q2 * norm, q3 * norm, q4 * norm
        self.heading = 0
        self.pitch = math.degrees(-math.asin(2.0 * (self.q[1] * self.q[3] - self.q[0] * self.q[2])))
        self.roll = math.degrees(math.atan2(2.0 * (self.q[0] * self.q[1] + self.q[2] * self.q[3]),
        self.q[0] * self.q[0] - self.q[1] * self.q[1] - self.q[2] * self.q[2] + self.q[3] * self.q[3]))


def mpu6050_init(i2c):
    i2c.writeto_mem(MPU6050_ADDR, MPU6050_PWR_MGMT_1, bytes([0]))


def combine_register_values(h, l):
    if not h[0] & 0x80:
        return h[0] << 8 | l[0]
    return -((h[0] ^ 255) << 8) | (l[0] ^ 255) + 1


def mpu6050_get_accel(i2c):
    accel_x_h = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1)
    accel_x_l = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_ACCEL_XOUT_L, 1)
    accel_y_h = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_ACCEL_YOUT_H, 1)
    accel_y_l = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_ACCEL_YOUT_L, 1)
    accel_z_h = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_ACCEL_ZOUT_H, 1)
    accel_z_l = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_ACCEL_ZOUT_L, 1)

    return [combine_register_values(accel_x_h, accel_x_l) / MPU6050_LSBG,
            combine_register_values(accel_y_h, accel_y_l) / MPU6050_LSBG,
            combine_register_values(accel_z_h, accel_z_l) / MPU6050_LSBG]


def mpu6050_get_gyro(i2c):
    gyro_x_h = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_GYRO_XOUT_H, 1)
    gyro_x_l = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_GYRO_XOUT_L, 1)
    gyro_y_h = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_GYRO_YOUT_H, 1)
    gyro_y_l = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_GYRO_YOUT_L, 1)
    gyro_z_h = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_GYRO_ZOUT_H, 1)
    gyro_z_l = i2c.readfrom_mem(MPU6050_ADDR, MPU6050_GYRO_ZOUT_L, 1)

    return [combine_register_values(gyro_x_h, gyro_x_l) / MPU6050_LSBDS,
            combine_register_values(gyro_y_h, gyro_y_l) / MPU6050_LSBDS,
            combine_register_values(gyro_z_h, gyro_z_l) / MPU6050_LSBDS]


led_post = Pin(2, Pin.OUT)
led_get = Pin(16, Pin.OUT)
relle = Pin(14, Pin.OUT)# D8 : Boot fails if pulled HIGH
dat = Pin(4)
ds = ds18x20.DS18X20(onewire.OneWire(dat))
roms = ds.scan()
print('found devices:', roms)

i2c = I2C(scl=Pin(12), sda=Pin(13))
encoder = Pin(5, Pin.IN)

wdt = WDT()
while True:
    try:
        mpu6050_init(i2c)
        tiempo_prev = 0
        ang_y_prev = 0
        fuse = Fusion()
        break
    except OSError:
        led_post.on()
        led_get.on()
        time.sleep_ms(500)
        led_post.off()
        led_get.off()
        time.sleep_ms(500)
        led_post.on()
        led_get.on()
        pass
    except NotImplementedError:
        led_post.on()
        led_get.on()
        time.sleep_ms(50)
        led_post.off()
        led_get.off()
        time.sleep_ms(50)
        led_post.on()
        led_get.on()
        pass

temp_pressure_url = 'http://iotcom.ir/monitor/add'
motor_url = 'http://iotcom.ir/monitor/motor'

temp = 0
angle = 0
count = 0
flag = 0
t1 = 0


def oserror():
    led_post.on()
    led_get.on()
    time.sleep_ms(500)
    led_post.off()
    led_get.off()
    time.sleep_ms(500)
    led_post.on()
    led_get.on()
    reset()


def notimplementerror():
    led_post.on()
    led_get.on()
    time.sleep_ms(500)
    led_post.off()
    led_get.off()
    time.sleep_ms(500)
    led_post.on()
    led_get.on()
    time.sleep_ms(500)
    led_post.off()
    led_get.off()
    time.sleep_ms(500)
    led_post.on()
    led_get.on()
    reset()


async def data():
    global temp, count, flag, angle
    while True:
        await asyncio.sleep_ms(10)
        try:
            led_get.off()
            fuse.update_nomag(mpu6050_get_accel(i2c), mpu6050_get_gyro(i2c))
            angle = int(fuse.pitch)
            ds.convert_temp()
            temp = int(ds.read_temp(roms[0]))

            if encoder.value() == 1:
                if flag == 0:
                    count += 1
                    flag = 1
            else:
                if flag == 1:
                    flag = 0

            led_get.on()
            wdt.feed()
        except OSError:
            led_post.on()
            time.sleep_ms(50)
            led_post.off()
            time.sleep_ms(50)
            pass
        except NotImplementedError:
            led_get.on()
            time.sleep_ms(50)
            led_get.off()
            time.sleep_ms(50)
            pass


async def server():
    global temp, count, angle, t1
    while True:
        await asyncio.sleep_ms(8000)
        try:
            led_post.off()
            speed = int(count / ((time.ticks_ms() - t1) / 1000))
            count = 0
            json = {'number': '1', 'temp': str(temp), 'pressure': '89995', 'speed': str(speed), 'angle': str(angle)}
            post_data = ujson.dumps(json)
            request.post(temp_pressure_url, data=post_data)
            led_post.on()
            wdt.feed()
        except OSError:
            oserror()
            pass
        except NotImplementedError:
            notimplementerror()
            pass

        time.sleep(1)

        try:
            led_get.off()
            get = request.get(motor_url)
            d = ujson.loads(get.text)
            status = d["status"]
            auto = d["auto"]
            current_time = d["time"]
            min_temp = d["min_temp"]
            max_temp = d["max_temp"]

            if status is True:
                relle.off()
            else:
                relle.on()
            get.close()
            led_get.on()
            wdt.feed()
            t1 = time.ticks_ms()
        except OSError:
            oserror()
            pass
        except NotImplementedError:
            notimplementerror()
            pass


async def main():
    task1 = asyncio.create_task(data())
    task2 = asyncio.create_task(server())
    await task1
    await task2

asyncio.run(main())