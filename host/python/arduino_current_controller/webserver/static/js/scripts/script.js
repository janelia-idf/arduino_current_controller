$(function() {
    var lights_on = false;
    var pwm_on = false;
    var percent_capacity = 40;
    var duration_on = 1000;
    var duration_off = 1000;

    $(':button').button();

    $("#sliderPercentCapacity").slider({
        range: "min",
        value: percent_capacity,
        min: 20,
        max: 100,
        step: 10
    });
    $("#displayPercentCapacity").text(percent_capacity + "%");
    $("#sliderPercentCapacity").on("slide",
                                   function( event, ui ) {
                                       percent_capacity = ui.value;
                                       $("#displayPercentCapacity").text(ui.value + "%");
                                       if (lights_on) {
                                           Sijax.request('turnOnAll',[percent_capacity]);
                                       } else if (pwm_on) {
                                           Sijax.request('startPwmAll',[percent_capacity,duration_on,duration_off]);
                                       };
                                   });

    $('#turnOnAll').click(function () {
        lights_on = true;
        pwm_on = false;
        Sijax.request('turnOnAll',[percent_capacity]);
    });
    $('#turnOffAll').click(function () {
        lights_on = false;
        pwm_on = false;
        Sijax.request('turnOffAll');
    });

    $("#sliderDurationOn").slider({
        range: "min",
        value: duration_on,
        min: 100,
        max: 2000,
        step: 200
    });
    $("#displayDurationOn").text(duration_on + " ms");
    $("#sliderDurationOn").on("slide",
                              function( event, ui ) {
                                  duration_on = ui.value;
                                  $("#displayDurationOn").text(ui.value + " ms");
                                  if (pwm_on) {
                                      Sijax.request('startPwmAll',[percent_capacity,duration_on,duration_off]);
                                  }
                              });
    $("#sliderDurationOff").slider({
        range: "min",
        value: duration_off,
        min: 100,
        max: 2000,
        step: 200
    });
    $("#displayDurationOff").text(duration_off + " ms");
    $("#sliderDurationOff").on("slide",
                               function( event, ui ) {
                                   duration_off = ui.value;
                                   $("#displayDurationOff").text(ui.value + " ms");
                                   if (pwm_on) {
                                       Sijax.request('startPwmAll',[percent_capacity,duration_on,duration_off]);
                                   }
                               });

    $('#startPwmAll').click(function () {
        lights_on = false;
        pwm_on = true;
        Sijax.request('startPwmAll',[percent_capacity,duration_on,duration_off]);
    });
    $('#stopPwmAll').click(function () {
        lights_on = false;
        pwm_on = false;
        Sijax.request('stopPwmAll');
    });

});
