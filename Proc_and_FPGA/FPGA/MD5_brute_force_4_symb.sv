module MD5_brute_force_4_symb(
            input logic           clk,
            input logic           ce,
            input logic [511 : 0] start_str,
            input logic [31 : 0]  a_MD5_hash,
            input logic [31 : 0]  b_MD5_hash,
            input logic [31 : 0]  c_MD5_hash,
            input logic [31 : 0]  d_MD5_hash,
            input logic           reset,
            input logic           reset_zero_string,
            output logic           find_str,
            output logic           symbols_done,
            output logic [511 : 0] result_str
            );



logic [511 : 0] new_str, str_for_next_module;
logic str_ready_1, str_ready_2;
logic [6:0] symb_num = 0;
logic str_ce = 0;
logic [13:0] str_cnt = 0;
logic restart_str_gen = 0;
logic ce_MD5_brute_force = 0;
logic symb_done_1, symb_done_2;
logic reset_other1, reset_other2, reset_other3 = 0;
logic zero_string = 1;
logic [511 : 0] res_str_1;
logic find_str_1; 

always_ff @ (posedge clk)
begin
  if(symb_num <= 95 && ce && (zero_string == 0))
  begin
    
    if(symb_num == 0)
    begin
      str_ce <= 1;
      restart_str_gen <= 0;
      symb_num <= symb_num + 1;
      reset_other1 <= 1;
    end
    else
    begin  
      if(symb_done_1)
      begin
        symb_num <= symb_num + 1;
        str_ce <= 1;
        reset_other1 <= 1;
      end
      else
      begin
        str_ce <= 0;
        reset_other1 <= 0;
        reset_other2 <= reset_other1;
        reset_other3 <= reset_other2;
      end 
    end
    
    str_for_next_module <= new_str;
    
  end
  else
  begin
    ce_MD5_brute_force <= 0;
    str_ce <= 0;
  end
  
  if(symb_num > 95)
  begin
    symbols_done <= 1;
    symb_num <= 0;
    restart_str_gen <= 1;
  end
  else
    symbols_done <= 0;
  
  if(str_ce)
    ce_MD5_brute_force <= 1;
  
  if(reset)
  begin
    str_ce <= 0;
    symb_num <= 0;
    restart_str_gen <= 1;
    reset_other1 <= 1;
	 symbols_done <= 0;
  end
  
  if(zero_string && ce && ~reset)
  begin
    str_for_next_module <= start_str; 
    if(symb_done_1)
    begin
      str_cnt <= 0;
      zero_string <= 0;
      ce_MD5_brute_force <= 0; 
    end
    else
    begin
      str_cnt <= str_cnt + 1;
      ce_MD5_brute_force <= 1; 
    end 
    
  end
  
end

string_gen StrG_2_1 (
        .clk(clk),
        .ce(str_ce),
        .old_string(start_str),
        .new_string(new_str),
        .restart(restart_str_gen),
        .from_num('b00100000), //32
        .to_num('b01111110),  //79
        .string_ready(str_ready_1)
);



MD5_brute_force_3_symb md5_br_f_2_1(.clk,
                           .ce(ce_MD5_brute_force),
                           .start_str(str_for_next_module),
                           .a_MD5_hash,
                           .b_MD5_hash,
                           .c_MD5_hash,
                           .d_MD5_hash,
                           .find_str(find_str_1),
                           .result_str(res_str_1),
                           .reset(reset_other3),
                           .reset_zero_string,
                           .symbols_done(symb_done_1));
                    


always_ff @ (posedge clk)
begin
  find_str <= find_str_1;
  result_str <= res_str_1;   
end


endmodule