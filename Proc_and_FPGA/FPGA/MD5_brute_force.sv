module MD5_brute_force(
            input logic           clk,
            input logic           ce,
            input logic [511 : 0] start_str,
            input logic [31 : 0]  a_MD5_hash,
            input logic [31 : 0]  b_MD5_hash,
            input logic [31 : 0]  c_MD5_hash,
            input logic [31 : 0]  d_MD5_hash,
            input logic           reset,
            output logic           find_str,
            output logic [511 : 0] result_str
            );


logic str_ce = 0;
logic [6:0] str_cnt = 0;
logic [511 : 0] new_str;
logic [511 : 0] md5_str;
logic str_ready;
logic [31:0] a_str, b_str, c_str, d_str;
logic [64:0][511:0] str_buf ;
logic restart_gen = 0;


always_ff @ (posedge clk)
begin
  if(ce && (str_cnt < 95))
  begin
    str_ce <= 1;
    str_cnt <= str_cnt + 1;
    restart_gen <= 0;
  end
  else
  begin
    str_ce <= 0;
    str_cnt <= 0;
    restart_gen <= 1;
  end
  
  if(reset)
  begin
    str_ce <= 0;
    str_cnt <= 0;
    restart_gen <= 1;
  end
  
end

string_gen StrG (
        .clk(clk),
        .ce(str_ce),
        .old_string(start_str),
        .new_string(new_str),
        .restart(restart_gen),
        .from_num('b00100000),
        .to_num('b01111110),
        .string_ready(str_ready)
);

always @ (posedge clk)
begin
  md5_str <= new_str;
  str_buf <= {str_buf[63:0],new_str};
end

MD5_Main_Steps MD5_and_StrG (
	.clk(clk), 
	.wb(md5_str), 
	.a0('h67452301), 
	.b0('hefcdab89), 
	.c0('h98badcfe), 
	.d0('h10325476), 
	.a64(a_str), 
	.b64(b_str), 
	.c64(c_str), 
	.d64(d_str)
);

logic [31:0] a_main, b_main, c_main, d_main; 
always_ff @ (posedge clk)
begin
  a_main <= a_str + 'h67452301;
  b_main <= b_str + 'hefcdab89;
  c_main <= c_str + 'h98badcfe;
  d_main <= d_str + 'h10325476;
  if((a_str + 'h67452301 == a_MD5_hash) && (b_str + 'hefcdab89 == b_MD5_hash) && (c_str + 'h98badcfe == c_MD5_hash) && (d_str + 'h10325476 == d_MD5_hash))
  begin
    find_str <= 1'b1;
    result_str <= str_buf[64];
  end
  else
  begin
    find_str <= 1'b0;
    result_str <= '0;
  end
end

endmodule