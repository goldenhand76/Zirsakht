var letter_modal = document.getElementById("modal_letter");
var structure_modal = document.getElementById("modal_structure");
var establish_modal = document.getElementById("modal_establish");


var letter = document.getElementById("add_letter");
var structure = document.getElementById("add_structure");
var establish = document.getElementById("add_establish");

var letter_close = document.getElementById("letter_close");
var structure_close = document.getElementById("structure_close");
var establish_close = document.getElementById("structure_close");


letter.onclick = function () {
    letter_modal.style.display = "block";
    document.getElementById("letter_subject").value = "";
    document.getElementById("letter_receiver").value = "";
    document.getElementById("letter_number").value = "";
    document.getElementById("letter_date").value = "";
    document.getElementById("letter_deadline").value = "";
    document.getElementById("letter_text").value = "";
};

structure.onclick = function () {
    structure_modal.style.display = "block";
    document.getElementById("structure_subject").value = "";
    document.getElementById("structure_responsible").value = "";
    document.getElementById("structure_number").value = "";
    document.getElementById("structure_date").value = "";
    document.getElementById("structure_deadline").value = "";
    document.getElementById("structure_text").value = "";
};

establish.onclick = function () {
    establish_modal.style.display = "block";
    document.getElementById("establish_subject").value = "";
    document.getElementById("establish_responsible").value = "";
    document.getElementById("establish_number").value = "";
    document.getElementById("establish_date").value = "";
    document.getElementById("establish_deadline").value = "";
    document.getElementById("establish_text").value = "";
};

letter_close.onclick = function () {
    letter_modal.style.display = "none";
};
structure_close.onclick = function () {
    structure_modal.style.display = "none";
};
establish_close.onclick = function () {
    establish_modal.style.display = "none";
};


window.onclick = function (event) {
    if (event.target === letter_modal) {
        letter_modal.style.display = "none";
    }
    if (event.target === structure_modal) {
        structure_modal.style.display = "none";
    }
    if (event.target === establish_modal) {
        establish_modal.style.display = "none";
    }
};

function add_letter() {
var url = $("#add_letter_url").attr("data-url");
$.ajax({
    url: url,
    type: "POST",
    data: {
        subject: $('#letter_subject').val(),
        receiver: $('#letter_receiver').val(),
        number: $('#letter_number').val(),
        date: $('#letter_date').val(),
        deadline: $('#letter_deadline').val(),
        text: $('#letter_text').val(),
    },
    success: function () {
        letter_modal.style.display = "none";
        location.reload()
    },
    error: function(status, exception) {
         window.alert(exception);
     },
});}

function add_structure() {
    var url = $("#add_structure_url").attr("data-url");
    $.ajax({
        url: url,
        type: "POST",
        data: {
            subject: $('#structure_subject').val(),
            responsible: $('#structure_responsible').val(),
            number: $('#structure_number').val(),
            date: $('#structure_date').val(),
            deadline: $('#structure_deadline').val(),
            text: $('#structure_text').val(),
        },
        success: function () {
            structure_modal.style.display = "none";
            location.reload()
        },
        error: function(status, exception) {
             window.alert(exception);
         },
    });}

function add_establish() {
var url = $("#add_establish_url").attr("data-url");
$.ajax({
    url: url,
    type: "POST",
    data: {
        subject: $('#establish_subject').val(),
        responsible: $('#establish_responsible').val(),
        number: $('#establish_number').val(),
        date: $('#establish_date').val(),
        deadline: $('#establish_deadline').val(),
        text: $('#establish_text').val(),
    },
    success: function () {
        establish_modal.style.display = "none";
        window.location.href = '../home/#tab-3';
        },
    error: function(status, exception) {
         window.alert(exception);
     },
});}