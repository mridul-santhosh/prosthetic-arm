// establish websocket connection
var socket = new WebSocket("ws://" + window.location.host + "/ws");

// set up chart
var chart = new SmoothieChart();
chart.streamTo(document.getElementById("chart"), 1000 /* delay */);

// set up data series
var line1 = new TimeSeries();

// add data series to chart
chart.addTimeSeries(line1, {lineWidth:2, strokeStyle:'#00ff00'});

// handle incoming websocket messages
socket.onmessage = function(event) {
  var data = JSON.parse(event.data);
  var timestamp = new Date(data.timestamp * 1000);
  var value = data.value;
  line1.append(timestamp, value);
};