"use strict";

var gradientChartOptionsConfigurationWithTooltipGreen = {
	maintainAspectRatio: false,
	legend: {
		display: false
	},
	tooltips: {
		backgroundColor: '#f5f5f5',
		titleFontColor: '#333',
		bodyFontColor: '#666',
		bodySpacing: 4,
		xPadding: 12,
		mode: "nearest",
		intersect: 0,
		position: "nearest"
	},
	responsive: true,
	scales: {
		yAxes: [{
			scaleLabel: {
				display: true,
				fontSize: 12,
				labelString: 'Temperature (C)'
			},
			ticks: {
				fontStyle: "500",
				beginAtZero: true,
				maxTicksLimit: 5,
				padding: 10
			},
			gridLines: {
				drawTicks: false,
				display: false
			}
		}],
		xAxes: [{
			barPercentage: 1.6,
			gridLines: {
				drawBorder: false,
				color: 'rgba(0,242,195,0.1)',
				zeroLineColor: "transparent",
			},
			ticks: {
				padding: 20,
				fontColor: "#9e9e9e"
			}
		}]
	}
};

// Gauge

var speed_opts = {
	angle: -0.2, // The span of the gauge arc
	lineWidth: 0.4, // The line thickness
	radiusScale: 1, // Relative radius
	pointer: {
		length: 0.50, // // Relative to gauge radius
		strokeWidth: 0.05, // The thickness
		color: '#ffffff' // Fill color
	},
	limitMax: true,     // If false, max value increases automatically if value > maxValue
	limitMin: true,     // If true, the min value of the gauge will be fixed
	colorStart: '#6F6EA0',   // Colors
	colorStop: '#C0C0DB',    // just experiment with them
	strokeColor: '#EEEEEE',  // to see which ones work best for you
	generateGradient: true,
	highDpiSupport: true,     // High resolution support
	// renderTicks is Optional
	renderTicks: {
		divisions: 12,
		divWidth: 1.1,
		divLength: 0.7,
		divColor: '#333333',
		subDivisions: 1,
		subLength: 0.5,
		subWidth: 1.3,
		subColor: '#666666'
	},
	percentColors: [[0.0, "#a9d70b"], [0.50, "#f9c802"], [1.0, "#ff0000"]],
	staticLabels: {
		font: "14px sans-serif",  // Specifies font
		labels: [0, 2, 4, 6, 8, 10, 12],  // Print labels at these values
		color: "#ffffff",  // Optional: Label text color
		fractionDigits: 0  // Optional: Numerical precision. 0=round off.
	},
	staticZones: [
		{strokeStyle: "#f7ff00", min: 0, max: 8}, // Red from 100 to 130
		{strokeStyle: "#00ff10", min: 8, max: 12}, // Green
	],

};
var temp_opts = {
	angle: -0.2, // The span of the gauge arc
	lineWidth: 0.4, // The line thickness
	radiusScale: 1, // Relative radius
	pointer: {
		length: 0.35, // // Relative to gauge radius
		strokeWidth: 0.035, // The thickness
		color: '#ffffff' // Fill color
	},
	limitMax: true,     // If false, max value increases automatically if value > maxValue
	limitMin: true,     // If true, the min value of the gauge will be fixed
	colorStart: '#6F6EA0',   // Colors
	colorStop: '#C0C0DB',    // just experiment with them
	strokeColor: '#EEEEEE',  // to see which ones work best for you
	generateGradient: true,
	highDpiSupport: true,     // High resolution support
	// renderTicks is Optional
	renderTicks: {
		divisions: 7,
		divWidth: 1.1,
		divLength: 0.7,
		divColor: '#333333',
		subDivisions: 5,
		subLength: 0.5,
		subWidth: 1.3,
		subColor: '#666666'
	},
	percentColors: [[0.0, "#a9d70b"], [0.50, "#f9c802"], [1.0, "#ff0000"]],
	staticLabels: {
		font: "14px sans-serif",  // Specifies font
		labels: [0, 10, 20, 40, 50, 60, 70],  // Print labels at these values
		color: "#ffffff",  // Optional: Label text color
		fractionDigits: 0  // Optional: Numerical precision. 0=round off.
	},
	staticZones: [
		{strokeStyle: "#1a9fef", min: 0, max: 30}, // Red from 100 to 130
		{strokeStyle: "#00ff10", min: 30, max: 60}, // Green
		{strokeStyle: "#ff0000", min: 60, max: 70}, // Yellow
	],

};

var angle_opts = {
	angle: -0.2, // The span of the gauge arc
	lineWidth: 0.4, // The line thickness
	radiusScale: 1, // Relative radius
	pointer: {
		length: 0.35, // // Relative to gauge radius
		strokeWidth: 0.035, // The thickness
		color: '#ffffff' // Fill color
	},
	limitMax: true,     // If false, max value increases automatically if value > maxValue
	limitMin: true,     // If true, the min value of the gauge will be fixed
	colorStart: '#6F6EA0',   // Colors
	colorStop: '#C0C0DB',    // just experiment with them
	strokeColor: '#EEEEEE',  // to see which ones work best for you
	generateGradient: true,
	highDpiSupport: true,     // High resolution support
	// renderTicks is Optional
	renderTicks: {
		divisions: 3,
		divWidth: 1.1,
		divLength: 1,
		divColor: '#333333',
		subDivisions: 5,
		subLength: 0.8,
		subWidth: 1.3,
		subColor: '#666666'
	},
	percentColors: [[0.0, "#a9d70b"], [0.50, "#f9c802"], [1.0, "#ff0000"]],
	staticLabels: {
		font: "14px sans-serif",  // Specifies font
		labels: [-75, -50, -25, 0],  // Print labels at these values
		color: "#ffffff",  // Optional: Label text color
		fractionDigits: 0.1,  // Optional: Numerical precision. 0=round off.
	},
	staticZones: [
		{strokeStyle: "#00087b", min: -75, max: -50}, // Red from 100 to 130
		{strokeStyle: "#0090ff", min: -50, max: 0}, // Yellow
	],

};

var speed = document.getElementById('speed-gauge'); // your canvas element
var temp = document.getElementById('temp-gauge'); // your canvas element
var angle = document.getElementById('angle-gauge'); // your canvas element
window.speed_gauge = new Gauge(speed).setOptions(speed_opts); // create sexy gauge!
window.temp_gauge = new Gauge(temp).setOptions(temp_opts); // create sexy gauge!
window.angle_gauge = new Gauge(angle).setOptions(angle_opts); // create sexy gauge!
document.getElementById('speed-gauge-textfield').className = "speed-gauge-textfield";
document.getElementById('temp-gauge-textfield').className = "temp-gauge-textfield";
document.getElementById('angle-gauge-textfield').className = "angle-gauge-textfield";
window.speed_gauge.setTextField(document.getElementById("speed-gauge-textfield"));
window.temp_gauge.setTextField(document.getElementById("temp-gauge-textfield"));
window.angle_gauge.setTextField(document.getElementById("angle-gauge-textfield"));
window.speed_gauge.maxValue = 12; // set max gauge value
window.temp_gauge.maxValue = 70; // set max gauge value
window.angle_gauge.maxValue = 0; // set max gauge value
window.speed_gauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
window.temp_gauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
window.angle_gauge.setMinValue(-75);  // Prefer setter over gauge.minValue = 0
window.speed_gauge.animationSpeed = 32; // set animation speed (32 is default value)
window.temp_gauge.animationSpeed = 32; // set animation speed (32 is default value)
window.angle_gauge.animationSpeed = 32; // set animation speed (32 is default value)

//Notify
$.notify({
	icon: 'flaticon-error',
	title: 'هشدار',
	message: 'احتمال یخ زدن کویل ها به دلیل افت دما',
},{
	type: 'info',
	placement: {
		from: "bottom",
		align: "right",
	},
	time: 1000,
});

//Chart AirConditioner

var tooltips = {
	bodySpacing: 4,
	mode: "nearest",
	intersect: 0,
	position: "nearest",
	xPadding: 10,
	yPadding: 10,
	caretPadding: 10
}

var layout = {
	padding: {left: 5, right: 5, top: 10, bottom: 10}
}

var ac_temp_chart_option = {
	responsive: true,
	maintainAspectRatio: false,
	legend: {
		display: false
	},
	tooltips: tooltips,
	layout: layout,
	scales: {
		yAxes: [{
			scaleLabel: {
				display: true,
				fontSize: 16,
				labelString: 'Temperature (C)'
			},
			ticks: {
				fontStyle: "500",
				beginAtZero: true,
				maxTicksLimit: 5,
				padding: 10
			},
			gridLines: {
				drawTicks: false,
				display: false
			}
		}],
		xAxes: [{
			type: 'time',
			distribution: 'linear',
			gridLines: {
				zeroLineColor: "transparent"
			},
			ticks: {
				autoSkip: true,
				source: 'auto',
				maxTicksLimit: 12,
				padding: 10,
				fontStyle: "500"
			}
		}]
	},
	legendCallback: function (chart) {
	var text = [];
	text.push('<ul class="' + chart.id + '-legend html-legend">');
	for (var i = 0; i < chart.data.datasets.length; i++) {
	text.push('<li><span style="background-color:' + chart.data.datasets[i].legendColor + '"></span>');
	if (chart.data.datasets[i].label) {
	text.push(chart.data.datasets[i].label);
}
text.push('</li>');
}
text.push('</ul>');
return text.join('');
}
}

var ac_speed_chart_option = {
	responsive: true,
	maintainAspectRatio: false,
	legend: {
	display: false
},
	tooltips: tooltips,
	layout: layout,
	scales: {
	yAxes: [{
		scaleLabel: {
			display: true,
			fontSize: 16,
			labelString: 'Speed (RPS)'
		},
		ticks: {
			fontStyle: "500",
			beginAtZero: true,
			maxTicksLimit: 5,
			padding: 10
		},
		gridLines: {
			drawTicks: false,
			display: false
		}
	}],
		xAxes: [{
			type: 'time',
			distribution: 'linear',
			gridLines: {
				zeroLineColor: "transparent"
			},
			ticks: {
				autoSkip: true,
				source: 'auto',
				maxTicksLimit: 12,
				padding: 10,
				fontStyle: "500"
			}
	}]
},
	legendCallback: function (chart) {
		var text = [];
		text.push('<ul class="' + chart.id + '-legend html-legend">');
		for (var i = 0; i < chart.data.datasets.length; i++) {
			text.push('<li><span style="background-color:' + chart.data.datasets[i].legendColor + '"></span>');
			if (chart.data.datasets[i].label) {
				text.push(chart.data.datasets[i].label);
			}
			text.push('</li>');
		}
		text.push('</ul>');
		return text.join('');
	}
}


function chart(key){
	$.ajax({
			type: "POST",
			url: "airconditioner/",
			data: {'device': key},
			success: function (response) {
				window.temp_gauge.set(response.temp_data[response.temp_data.length - 1].y);
				window.speed_gauge.set(response.speed_data[response.speed_data.length - 1].y);
				window.angle_gauge.set(response.angle_data[response.angle_data.length - 1].y);

				var temp_ctx = document.getElementById('temp-chart').getContext('2d');
				var speed_ctx = document.getElementById('speed-chart').getContext('2d');

				if (window.chart1 !== undefined)
					window.chart1.destroy();
				if (window.chart2 !== undefined)
					window.chart2.destroy();

				window.chart1 = new Chart(temp_ctx, {
					type: 'line',
					data: {
						labels: [],
						datasets: [{
							label: 'AirConditioner1',
							borderColor: '#f3545d',
							pointBackgroundColor: 'rgba(243, 84, 93, 0.6)',
							pointRadius: 0,
							backgroundColor: 'rgba(243, 84, 93, 0.4)',
							legendColor: '#f3545d',
							fill: true,
							borderWidth: 2,
							data: response.temp_data,
						}]
					},
					options: ac_temp_chart_option,
				});
				var myLegendContainer = document.getElementById("myChartLegend");
				myLegendContainer.innerHTML = window.chart1.generateLegend();
				var legendItems = myLegendContainer.getElementsByTagName('li');
				for (var i = 0; i < legendItems.length; i += 1) {
					legendItems[i].addEventListener("click", legendClickCallback, false);
				}

				window.chart2 = new Chart(speed_ctx, {
					type: 'line',
					data: {
						labels: [],
						datasets: [{
							label: 'AirConditioner1',
							borderColor: '#54e6f3',
							pointBackgroundColor: 'rgb(84,161,243)',
							pointRadius: 0,
							backgroundColor: 'rgb(84,142,243)',
							legendColor: '#54bbf3',
							fill: true,
							borderWidth: 2,
							data: response.speed_data,
						}]
					},
					options: ac_speed_chart_option
				});
				var myLegendContainer2 = document.getElementById("myChartLegend2");
				myLegendContainer2.innerHTML = window.chart2.generateLegend();
				var legendItems2 = myLegendContainer2.getElementsByTagName('li');
				for (var i = 0; i < legendItems.length; i += 1) {
					legendItems2[i].addEventListener("click", legendClickCallback, false);
				}
			}
		});
}


$(document).ready(function () {
	setTimeout(function () {
		$.ajax({
			type: "GET",
			url: "weather/",
			success: function (response) {
				var hourly_ctx = document.getElementById('hourlyforecast').getContext('2d');
				var daily_ctx = document.getElementById('dailyforecast').getContext('2d');

				var gradientStroke = hourly_ctx.createLinearGradient(0, 230, 0, 50);
				gradientStroke.addColorStop(1, 'rgba(66,134,121,0.15)');
				gradientStroke.addColorStop(0.4, 'rgba(66,134,121,0.0)'); //green colors
				gradientStroke.addColorStop(0, 'rgba(66,134,121,0)'); //green colors

				var data1 = {
					labels: response.half_day_dates,
					datasets: [{
						label: "temperature",
						fill: true,
						backgroundColor: gradientStroke,
						borderColor: '#00d6b4',
						borderWidth: 2,
						borderDash: [],
						borderDashOffset: 0.0,
						pointBackgroundColor: '#00d6b4',
						pointBorderColor: 'rgba(255,255,255,0)',
						pointHoverBackgroundColor: '#00d6b4',
						pointBorderWidth: 20,
						pointHoverRadius: 4,
						pointHoverBorderWidth: 15,
						pointRadius: 4,
						data: response.half_day_temps,
					}]
				};
				var myChart1 = new Chart(hourly_ctx, {
					type: 'line',
					data: data1,
					options: gradientChartOptionsConfigurationWithTooltipGreen
				});

				var data2 = {
					labels: response.five_day_dates,
					datasets: [{
						label: "minimum temperature",
						fill: true,
						borderColor: '#00d6b4',
						borderWidth: 2,
						borderDash: [],
						borderDashOffset: 0.0,
						pointBackgroundColor: '#00d6b4',
						pointBorderColor: 'rgba(255,255,255,0)',
						pointHoverBackgroundColor: '#00d6b4',
						pointBorderWidth: 20,
						pointHoverRadius: 4,
						pointHoverBorderWidth: 15,
						pointRadius: 4,
						data: response.five_day_min_temps,
					}, {
						label: "maximum temperature",
						fill: true,
						borderColor: '#00d6b4',
						borderWidth: 2,
						borderDash: [],
						borderDashOffset: 0.0,
						pointBackgroundColor: '#00d6b4',
						pointBorderColor: 'rgba(255,255,255,0)',
						pointHoverBackgroundColor: '#00d6b4',
						pointBorderWidth: 20,
						pointHoverRadius: 4,
						pointHoverBorderWidth: 15,
						pointRadius: 4,
						data: response.five_day_max_temps,
					}]
				};
				var myChart2 = new Chart(daily_ctx, {
					type: 'line',
					data: data2,
					options: gradientChartOptionsConfigurationWithTooltipGreen
				});
			}
		})
	}, 1000)
})





var room_temp_chart_tooltips = {
	bodySpacing: 4,
	mode: "nearest",
	intersect: 0,
	position: "nearest",
	xPadding: 10,
	yPadding: 10,
	caretPadding: 10
}

var room_temp_chart_layout = {
	padding: {left: 10, right: 10, top: 10, bottom: 10}
}

var room_temp_chart_option = {
	maintainAspectRatio: true,
	tooltips: room_temp_chart_tooltips,
	layout: room_temp_chart_layout,
	scales: {
		yAxes: [{
			scaleLabel: {
				display: true,
				fontSize: 12,
				labelString: 'Temperature (C)'
			},
			ticks: {
				fontStyle: "500",
				beginAtZero: true,
				maxTicksLimit: 5,
				padding: 10
			},
			gridLines: {
				drawTicks: false,
				display: false
			}
		}],
		xAxes: [{
			type: 'time',
			distribution: 'linear',
			gridLines: {
				zeroLineColor: "transparent"
			},
			ticks: {
				autoSkip: true,
				source: 'auto',
				maxTicksLimit: 12,
				padding: 10,
				fontStyle: "500"
			}
		}]
	},
}
$(document).ready(function () {
	setTimeout(function () {
		$.ajax({
			type: 'POST',
			url: "room-chart/",
			data:{'room': 611},
			success: function (response) {
				var room_temp_ctx = document.getElementById('room-temp-chart').getContext('2d');
				if (window.room_temp_chart !== undefined)
					window.room_temp_chart.destroy();
				window.room_temp_chart = new Chart(room_temp_ctx, {
					type: 'line',
					data: {
						labels: [],
						datasets: [{
							label: 'Room Temp',
							borderColor: "rgb(145, 245, 85, 1)",
							pointRadius: 0,
							legendColor: '#33621c',
							fill: true,
							borderWidth: 2,
							data: response.temp_data
						},
							{label: 'Set Temp',
							borderColor: "rgb(0, 255, 255, 0.3)",
							pointRadius: 0,
							legendColor: '#1c624e',
							fill: true,
							borderWidth: 2,
							data: response.set_data
							}]
					},
					options:room_temp_chart_option,
				});
			}
		});
	}, 1000)
})





// Interval
$(document).ready(function () {
	setInterval(function () {
		setTimeout(function (){
			for (i=0; i < $("#tbody2 tr").length; i++) {
				if ($("#tbody2 tr")[i].className == 'selected') {
					var value = $("#tbody2 tr")[i].getAttribute('motor');
				}
			}
			var device = 'AC' + value;
			$.ajax({
				type: "POST",
				url: "airconditioner/",
				data: {'device': device},
				success: function (response) {
					try {
						window.chart1.data.datasets[0].data = response.temp_data;
						window.chart1.data.labels = [];
						window.chart1.update();

						window.chart2.data.datasets[0].data = response.speed_data;
						window.chart2.data.labels = [];
						window.chart2.update();

						window.temp_gauge.set(response.temp_data[response.temp_data.length - 1].y);
						window.speed_gauge.set(response.speed_data[response.speed_data.length - 1].y);
						window.angle_gauge.set(response.angle_data[response.angle_data.length - 1].y);
					}catch (err){
					}


					console.log()

				}
			});
		}, 1000);

		setTimeout(function (){
			$.ajax({
				type: "POST",
				url: "monitor/motor",
				data: {},
				success: function (response) {
					var status = response.status;
					var auto = response.auto;
					document.getElementById("ac-status").checked = status === true;
					document.getElementById("ac-auto").checked = auto === true;
				}
			});
		}, 1000)

		setTimeout(function () {
		$.ajax({
			type: 'POST',
			url: "room-chart/",
			data:{'room': 611},
			success: function (response) {
				window.room_temp_chart.data.datasets[0].data = response.temp_data;
				window.room_temp_chart.data.datasets[1].data = response.set_data;
				window.room_temp_chart.data.labels = [];
				window.room_temp_chart.update();
				}
			});
		}, 1000)

	}, 5000);
})
