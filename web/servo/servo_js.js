var sliders = document.querySelectorAll('.servo-slider');

// Loop through sliders and add change event listener
for (var i = 0; i < sliders.length; i++) {
  sliders[i].addEventListener('change', function() {
    var servoNum = this.dataset.servo;
    var servoPos = this.value;

    // Send servo position to server
    var data = {};
    data[servoNum] = servoPos;
    ws.send(JSON.stringify(data));

    // Update servo position display
    var posDisplay = document.querySelector('.servo-pos[data-servo="' + servoNum + '"]');
    posDisplay.innerHTML = servoPos;
  });
}
