module MD5_brute_force_2_symb(
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
logic [6:0] str_cnt = 0;
logic restart_str_gen = 0;
logic ce_MD5_brute_force = 0;
logic reset_1, reset_2;
logic zero_string = 1;
logic [511 : 0] res_str_1;
logic find_str_1; 
//logic restart_cnt = 0;

always_ff @ (posedge clk)
begin
  reset_1 <= reset;
  reset_2 <= reset_1;
end

always_ff @ (posedge clk)
begin
  if(symb_num <= 94 && ce && (zero_string == 0))
  begin
    symbols_done <= 0;
    if(str_cnt == 0)
    begin
      str_ce <= 1;
      str_cnt <= str_cnt + 1;
      restart_str_gen <= 0;
    end
    else
    begin
      str_ce <= 0;
      if(str_cnt == 95)
      begin
        str_cnt <= 0;
        symb_num <= symb_num + 1;
      end
      else
        str_cnt <= str_cnt + 1;
    end
    
    str_for_next_module <= new_str;
    
  end
  
  if(symb_num > 95 && ~ce)
    ce_MD5_brute_force <= 0;
    
  //////////////////////////////
  if(symb_num > 94)
  begin
    symbols_done <= 1;
    symb_num <= 0;
    restart_str_gen <= 1;
  end
  //////////////////////////////
  if(str_ce)
    ce_MD5_brute_force <= 1;
    
  if(reset_zero_string)
    zero_string <= 1;
    
  if(reset)
  begin
    str_ce <= 0;
    str_cnt <= 0;
    symb_num <= 0;
    restart_str_gen <= 1; 
    symbols_done <= 0; 
  end
  
  if(zero_string && ce && ~reset)
  begin
    str_for_next_module <= start_str; 
    //tak kak na dva takta ranshe zakanchivaet
    if(str_cnt == 97)
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

string_gen StrG_1 (
        .clk(clk),
        .ce(str_ce),
        .old_string(start_str),
        .new_string(new_str),
        .restart(restart_str_gen),
        .from_num('b00100000), //32
        .to_num('b01111110),  //79
        .string_ready(str_ready)
);


MD5_brute_force md5_br_f_1(.clk,
                           .ce(ce_MD5_brute_force),
                           .start_str(str_for_next_module),
                           .a_MD5_hash,
                           .b_MD5_hash,
                           .c_MD5_hash,
                           .d_MD5_hash,
                           .find_str(find_str_1),
                           .reset(reset_2),
                           .result_str(res_str_1));

always_ff @ (posedge clk)
begin
  find_str <= find_str_1;
  result_str <= res_str_1; 
end

endmodule