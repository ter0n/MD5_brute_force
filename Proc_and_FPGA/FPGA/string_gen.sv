//будет добавляться только 1 символ
module string_gen
(
	input logic [511:0] old_string,
	input logic [7:0] from_num, //номер, от которого начинаем
	input logic [7:0] to_num, //номер, которым заканчиваем
	input logic clk,
	input logic ce,
	input logic restart,
	output logic [511:0] new_string,
	output logic string_ready);
   
	logic [7 : 0] count = '0;
	logic [7 : 0] new_symb = '0;
	logic new_symb_ready = 0;
	
	always_ff @ (posedge clk)
	begin
	  if(restart)
	  begin
	    count <= '0;
	    new_symb_ready <= 0;  
	    new_symb <= '0;
	  end
	
	  if(ce)
	  begin
	    new_symb <= from_num + count;
		 new_symb_ready <= 1;
		 count <= count + 1;
	  end
		
	  if(new_symb_ready && (new_symb <= to_num))
	  begin
	  	 new_string <= {old_string[439:0], new_symb};
		 //new_string[511 : 448] <= old_string[511 : 448] + 1;
		 new_string[511 : 451] <= old_string[511 : 451] + 1; //????? ?? 3 ???? ?????, ?????? -- ?? ????
		 string_ready <= 1;
	  end
	  else
	    string_ready <= 0;
		
	end

endmodule