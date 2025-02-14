function sort_letter(field){
    var ascending = $('#ascending').val();
    var url = $("#sort_letter_url").attr("data-url");
    $.ajax({
        url: url,
        type: "POST",
        data: {
            field: field,
            ascending: ascending,
        },
        success: function (data) {
            $('#letter').html(data);

            if ($('#ascending').val() === 'true') {
                document.getElementById("ascending").value = "false";
            } else {
                document.getElementById("ascending").value = "true";
            }
        },
    });
}


function sort_repair(field) {
    var ascending = $('#ascending').val();
    var url = $("#sort_repair_url").attr("data-url");
    $.ajax({
        url: url,
        type: "POST",
        data: {
            device: $('#code').text(),
            field: field,
            ascending: ascending,
        },
        success: function (response) {
            var repairs = JSON.parse(response["repairs"]);
            $("#tbody").empty();
            for (i = 0; i < repairs.length; i++) {
                var field = repairs[i]["fields"];
                var date = new Date(field["date"]);
                $("#tbody").append("<tr><td style='text-align: right'><a style=\"font-family: 'B Nazanin'\"  type=\"button\" href=\"../edit_repair/" + repairs[i]["pk"] + "/\">" + field["explain"] +
                    "</a></td><td style=\"font-family: 'B Nazanin'\" >" + field["repairman"] +
                    "</td><td style=\"font-family: 'B Nazanin'\" >" + date.toLocaleDateString('fa-IR') + "</td></tr>");
            }
            if ($('#ascending').val() === 'true') {
                document.getElementById("ascending").value = "false";
            } else {
                document.getElementById("ascending").value = "true";
            }
        },
    });
}

var modal = document.getElementById("myModal");
var btn = document.getElementById("myBtn");
var span = document.getElementById("close");


btn.onclick = function () {
    modal.style.display = "block";
    document.getElementById("repair_man").value = "";
    document.getElementById("repair_explain").value = "";
    document.getElementById("test_date_id").value = "";

};
span.onclick = function () {
    modal.style.display = "none";
};
window.onclick = function (event) {
    if (event.target === modal) {
        modal.style.display = "none";
    }
};

function add_repair() {
    var url = $("#add_repair_url").attr("data-url");

    $.ajax({
        url: url,
        type: "POST",
        data: {
            repair_device: $('#code').text(),
            repair_man: $('#repair_man').val(),
            repair_explain: $('#repair_explain').val(),
            repair_date: $('#test_date_id').val(),
        },
        success: function () {
            modal.style.display = "none";
            $("#select").change();
        },
        error: function(status, exception) {
             window.alert(exception);
         },
    });
}