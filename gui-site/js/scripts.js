// With JQuery
$('#testSlider').slider({
	formatter: function(value) {
        console.log('Current value: ' + value);
        $('#testResult').text('Current value: ' + value);
	}
});

