/*
	Read Only by HTML5 UP
	html5up.net | @n33co
	Free for personal and commercial use under the CCA 3.0 license (html5up.net/license)
*/

(function($) {

	skel.breakpoints({
		xlarge: '(max-width: 1680px)',
		large: '(max-width: 1280px)',
		medium: '(max-width: 1024px)',
		small: '(max-width: 736px)',
		xsmall: '(max-width: 480px)'
	});

	$(function() {

		var $body = $('body'),
			$header = $('#header'),
			$nav = $('#nav'), $nav_a = $nav.find('a'),
			$wrapper = $('#wrapper');

		// Fix: Placeholder polyfill.
			$('form').placeholder();

		// Prioritize "important" elements on medium.
			skel.on('+medium -medium', function() {
				$.prioritize(
					'.important\\28 medium\\29',
					skel.breakpoint('medium').active
				);
			});

		// Header.
			var ids = [];

			// Set up nav items.
				$nav_a
					.scrolly({ offset: 44 })
					.on('click', function(event) {

						var $this = $(this),
							href = $this.attr('href');

						// Not an internal link? Bail.
							if (href.charAt(0) != '#')
								return;

						// Prevent default behavior.
							event.preventDefault();

						// Remove active class from all links and mark them as locked (so scrollzer leaves them alone).
							$nav_a
								.removeClass('active')
								.addClass('scrollzer-locked');

						// Set active class on this link.
							$this.addClass('active');

					})
					.each(function() {

						var $this = $(this),
							href = $this.attr('href'),
							id;
						// Not an internal link? Bail.
							if (href.charAt(0) != '#')
								return;

						// Add to scrollzer ID list.
							id = href.substring(1);
							$this.attr('id', id + '-link');
							ids.push(id);

					});

			// Initialize scrollzer.
				$.scrollzer(ids, { pad: 300, lastHack: true });

		// Off-Canvas Navigation.

			// Title Bar.
				$(
					'<div id="titleBar">' +
						'<a href="#header" class="toggle"></a>' +
						'<span class="title">' + "Агрокомплекс" + '</span>' +
					'</div>'
				)
					.appendTo($body);

			// Header.
				$('#header')
					.panel({
						delay: 500,
						hideOnClick: true,
						hideOnSwipe: true,
						resetScroll: true,
						resetForms: true,
						side: 'right',
						target: $body,
						visibleClass: 'header-visible'
					});

			// Fix: Remove navPanel transitions on WP<10 (poor/buggy performance).
				if (skel.vars.os == 'wp' && skel.vars.osVersion < 10)
					$('#titleBar, #header, #wrapper')
						.css('transition', 'none');

	});

})(jQuery);
/*
(function($) {
	document.getElementById("btn_add_node").addEventListener("click", function() {
		f = document.getElementById("form_add_node");
		if (f.style.visibility == "hidden")
			f.style.visibility = "visible";
		else
			f.style.visibility = "hidden";

	}, false);
})(jQuery);
(function addTo (dest, teg, content) {
	var elem = document.createElement(teg);
	elem.innerHTML = content;
	dest.appendChild(elem);
});

(function($) {
	document.getElementById("form_add_node").addEventListener("submit", function (evt) {
	evt.preventDefault();
	nocache = "&nocache" + Math.random() *1000000;
	var request = new XMLHttpRequest();
	var node_id = document.getElementById("node_id").value;
	var node_name = document.getElementById("node_name").value;
	var plant_name = document.getElementById("plant_name").value;
	var node_type = document.getElementById("node_type").value;
	request.onreadystatechange = function() {
			if (this.readyState == 4 && 
			this.status == 200 && 
			this.responseText != null) {
				var data = this.responseText;
				if (data == "FUCKING INCREDIBLE MAGIC") {
					table = getElementById("node_table");
					var tr = document.createElement('tr');
					addTo(tr, "th", node_id);
					addTo(tr, "th", node_name);
					addTo(tr, "th", plant_name);
					addTo(tr, "th", node_type);
					table.appendChild(tr);
				} else {
					alert("Случилась ошибка на арде. Это повод выпить!");
				}
			}
	};
	request.onerror = function() {
		alert("Случилась ошибка сети. Это повод выпить!");
	};
	request.open("GET", "Sadd_node" + nocache + "&node_id="+node_id+"&node_name="+node_name+"&plant_name="+plant_name+"&node_type="+node_type, false);
	request.send();
}, false);
})(jQuery);
*/

(function($) {
	fields = ["s", "t", "d"]
	document.getElementById("form_add_node").addEventListener("submit", function (evt) {
	evt.preventDefault();
	nocache = "&nocache" + Math.random() *1000000;
	var request = new XMLHttpRequest();
	form = document.getElementById("form_add_node");
	divs = form.getElementsByTagName("div");
	arr = [];
	for (i = 0; i < 3; i++){
		arr.push([]);
		for (j = 0; j < 3; j++) {
			arr[i].push(divs[j].getElementsByTagName("input")[0].value);
		}
	}
	request.onreadystatechange = function() {
			if (this.readyState == 4 && 
			this.status == 200 && 
			this.responseText != null) {
				var data = this.responseText;
				if (data == "FUCKING INCREDIBLE MAGIC") {
					alert("ДА!")
				}
	};
}
	request.onerror = function() {
		alert("Случилась ошибка сети. Это повод выпить!");
	};
	req = "ajax_query" + nocache;
	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++) {
			req += "&" + fields[i] + i + "=" + arr[j][i];
		}
	} 
	req += "&";
	alert(req);
	request.open("GET", req, false);
	request.send();
}, false);
})(jQuery);