$(document).ready(function(){
	
	$("object").each(function(){
		var results_file_name=$(this).attr("data");
		var expected_file_name=results_file_name.replace(/results/,"expected");
		
		//var results_data=$(this).contents().html();
		//var expected_data;
		
		console.log("FILE NAME IS:"+results_file_name+" and EXPECTED IS:"+expected_file_name);
		
		//console.log("RESULT TEXT IS:"+results_data.substr(0,100));
		//$.ajax({
		//	url:results_file_name,
		//	dataType:"text",
		//	success:function(data){
		//		result_data=data;
		//		console.log("GOT RESULT for "+results_file_name);
		//	}
		//})
	});
	
});

