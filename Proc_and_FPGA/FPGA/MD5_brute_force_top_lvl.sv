module MD5_brute_force_top_lvl(
            input logic           	clk,
            input logic           	ce,
            input logic [31 : 0]  	a_MD5_hash,
            input logic [31 : 0]  	b_MD5_hash,
            input logic [31 : 0]  	c_MD5_hash,
            input logic [31 : 0]  	d_MD5_hash,
            input logic           	reset,
            output logic           	find_str,
            output logic [511 : 0] 	result_str,
			output logic [3   : 0] 	symb_count,
			output logic 			end_brute_force
            );


logic [511:0] 	start_string = 'b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000;				
logic 			md5_br = 0;
logic 			find_str_1;
logic [511:0] res_str;
logic symb_done;
logic reset_zero_string;
logic [3 : 0] 	symb_count_1;


always_ff @(posedge clk)
begin
  if(ce)
    if(symb_done)
      md5_br <= 0;
    else
      md5_br <= 1;
  else
    md5_br <= 0;
end
								  
MD5_brute_force_4_symb md5_br_f_4(.clk(clk),
                                .ce(md5_br),
                                .start_str(start_string),
                                .a_MD5_hash,
                                .b_MD5_hash,
                                .c_MD5_hash,
                                .d_MD5_hash,
                                .find_str(find_str_1),
                                .result_str(res_str),
                                .symbols_done(symb_done),
                                .reset_zero_string(reset),
								.reset,
								);
							  	
always_ff @ (posedge clk)
begin
  
  if(reset)
  begin
    symb_count <= 0;
		find_str <= 0;
		result_str <= '0;
  end

  
	if (find_str_1) 
	begin
		symb_count <= 4;
		find_str <= 1;
		result_str <= res_str;
	end
	
	/*
	if (symb_done ) 
	begin
		symb_count <= 6'b1111;
		end_brute_force <= 1;
	end
	else
	   end_brute_force <= 0;
		*/
end							
endmodule