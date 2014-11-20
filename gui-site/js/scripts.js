// With JQuery
$('#testSlider').slider({
    formatter: function(value) {
       $('.min-slider-handle').text(value);
       return value;
    },

});

$(window).load(function(){
  $('.min-slider-handle').text('15').css('text-align', 'center');
});
