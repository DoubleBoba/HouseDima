function loadArdData($) {
	nocache = "&nocache" + Math.Math.random() *1000000;
	var request = new XMLHttpRequest();
	function parseLiter (strArr, liter) {
	var result = [];
	var counter = -1;
	for (i = 0; i < strArr.length; i++) {
		if (strArr[i][0] == liter) {
			counter++;
			result.push([]);
		}
		result[counter].push(strArr[i]);
	}
	return result;
}
	request.onreadystatechange = function () {
		if (this.readyState == 4 && 
			this.status == 200 && 
			this.responseText != null) {
			var data_raw = this.responseText.split("\n");
			var plants_raw = parseLiter(data_raw, "n");
			
			var plants = []
			plants_raw.forEach(function(element, index){
				var plant = new Object();
				getProp = function(arr){
					e = arr.shift();
					return e.slice(2, e.indexOf(";")-1);
				}
				plant.name = getProp(element);
				plant.descr = getProp(element);
				plant.alg = getProp(element);
				nodes = parseLiter(element, 'u');
				plant.nodes = [];
				nodes.forEach(function(element1, index){
					var node = new Object();
					node.name = getProp(element1);
					node.vcc = getProp(element1);
					var sensors = parseLiter(element1, "s");
					node.sensors = [];
					sensors.forEach(function(sensStrs, index){
						var sensor = new Object();
						sensor.name = getProp(sensStrs);
						sensor.value = getProp(sensStrs);
						node.sensors.push(sensor);
					});
					plant.nodes.push(node);
				});
				plants.push(plant);
			});
			
		}
	}
	request.open("GET", "load_all_plants" + nocache, true);
	request.send(null);
	setTimeout("loadArdData()", 3000);
}