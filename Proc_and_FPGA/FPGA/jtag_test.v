//module
//receives BYTE from JTAG and displays it on doard LEDs
//TCL script may read state of board KEYs
module jtag_test(
  input wire clock50Mhz, 
  input wire [1:0]key,
  output reg [7:0]led
  );

// Signals and registers declared for Virtual JTAG instance
wire tck, tdi;
wire cdr, eldr, e2dr, pdr, sdr, udr, uir, cir, tms;
reg  tdo;
wire [3:0]ir_in; //IR command register

// Instantiation of Virtual JTAG
myjtag myjtag_inst(
.tdo (tdo),
.tck (tck),
.tdi (tdi),
.ir_in(ir_in),
.ir_out(),
.virtual_state_cdr (cdr),
.virtual_state_e1dr(e1dr),
.virtual_state_e2dr(e2dr),
.virtual_state_pdr (pdr),
.virtual_state_sdr (sdr),
.virtual_state_udr (udr),
.virtual_state_uir (uir),
.virtual_state_cir (cir)
);

//md5-part
//-----------------------------------------------------------------------------------------------
reg clk, reset, test_all, MD5_ready;
wire [31:0] a, b, c, d;
reg [31:0] count = 0;
reg [511:0] chunk = 'b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000;
//надо будет сделать нормальную тактовую, но пока пусть будет так
always @(posedge tck)
	clk <= tck;


//-----------------------------------------------------------------------------------------------

//data receiver
//shifts incoming data during PUSH command
reg [31:0] MD5_A = 'h00000001, MD5_B = 'h00000002, MD5_C = 'h00000003, MD5_D = 'h00000004;
reg [31:0] shift_dr_in_1, shift_dr_in_2, shift_dr_in_3, shift_dr_in_4;
reg [4:0]shift_dr_in_5;

reg ce_A = 'b0, ce_B = 'b0, ce_C = 'b0, ce_D = 'b0;

always @(posedge tck)
begin
   if(sdr && (ir_in == 4'b0110) )
	begin
     shift_dr_in_1 <= { tdi, shift_dr_in_1[31:1] };
	end
	  
	if(sdr && (ir_in == 4'b0111) )
	begin
     shift_dr_in_2 <= { tdi, shift_dr_in_2[31:1] };
	end
	  
	if(sdr && (ir_in == 4'b1000) )
	begin
     shift_dr_in_3 <= { tdi, shift_dr_in_3[31:1] };
	end
	  
	if(sdr && (ir_in == 4'b1001) )
	begin
     shift_dr_in_4 <= { tdi, shift_dr_in_4[31:1] };
	end
	
	if(sdr && (ir_in == 4'b0101) )
	begin
     shift_dr_in_5 <= { tdi, shift_dr_in_5[4:1] };
	end
	
end

	
//data receiver
//write received data (during PUSH command) into LED register 
/*
always @(posedge tck)
begin
   if(udr && (ir_in==4'b0110) )
	begin
     MD5_A <= shift_dr_in_1;
	  ce_A <= 'b1;
	  led[0] <= 'b1;
	end
	
	if(udr && (ir_in==4'b0111) )
	begin
     MD5_B <= shift_dr_in_2;
	  ce_B <= 'b1;
	  led[1] <= 'b1;
	end
	  
	if(udr && (ir_in==4'b1000) )
	begin
     MD5_C <= shift_dr_in_3;
	  ce_C <= 'b1;
	  led[2] <= 'b1;
	end
	  
	if(udr && (ir_in==4'b1001) )
	begin
     MD5_D <= shift_dr_in_4;
	  ce_D <= 'b1;
	  led[3] <= 'b1;
	  md5_br <= 'b1;
	end
	
	if(find_str || end_brute_force)
	begin
	  md5_br <= 'b0;
	  led[5] <= 'b1;
	end
	
end
*/
wire find_str;
wire [511:0] res_str;
wire end_brute_force;
reg md5_br;
wire [3:0] symb_count; 
reg  [3:0] symb_count_reg; 

initial
begin
  /*
  find_str = 1'b0;
  res_str = {512{1'b0}};
  end_brute_force = 1'b0;
  symb_count = 6'b000000;
  */
  md5_br = 1'b0;
  symb_count_reg = 'b000000;
end  

    
MD5_brute_force_top_lvl md5_br_f(.clk(clock50Mhz),
                                .ce(md5_br),
                                .a_MD5_hash(MD5_A),
                                .b_MD5_hash(MD5_B),
                                .c_MD5_hash(MD5_C),
                                .d_MD5_hash(MD5_D),
										  .reset(reset),
                                .find_str(find_str),
                                .result_str(res_str),
                                .symb_count(symb_count),
										  .end_brute_force(end_brute_force));


//data sender
reg [31:0]shift_dr_out_1, shift_dr_out_2, shift_dr_out_3, shift_dr_out_4;
reg [3:0]shift_dr_out_6 = 'b0000;
reg [7:0] shift_dr_out_5 = 'h00;
reg [511:0] result_string = 'b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
reg [3:0] reset_cntr;
reg		set_res_str = 'b0;
/*
always @(posedge clock50Mhz)
begin
	if(find_str)
	begin
	  result_string <= res_str;
	  symb_count_reg <= symb_count;
	end
	if(end_brute_force)
	begin
	  symb_count_reg <= symb_count;
	end
end
*/
reg [4:0] new_ce = 5'b00000;
always @(posedge tck)
begin
   if(udr && (ir_in==4'b0110) )
     MD5_A <= shift_dr_in_1;	
	if(udr && (ir_in==4'b0111) )
     MD5_B <= shift_dr_in_2;	  
	if(udr && (ir_in==4'b1000) )
     MD5_C <= shift_dr_in_3;	  
	if(udr && (ir_in==4'b1001) )
     MD5_D <= shift_dr_in_4;	
	  
	if(udr && (ir_in==4'b0101) )
     new_ce <= shift_dr_in_5;
	if(new_ce == 5'b10101)
	begin
		md5_br <= 1;
		reset <= 1;
		reset_cntr <= 0;
	end

	if(reset == 1)
	begin
		reset_cntr <= reset_cntr + 1;
		set_res_str <= 0;
	end
	if(reset_cntr == 4'b1111)
	begin
		reset <= 0;
		new_ce <= 5'b00000;
	end

	if(cdr && (ir_in == 4'b1010))
		shift_dr_out_6 <= symb_count_reg;
	else
	if(sdr && (ir_in==4'b1010))
		shift_dr_out_6 <= { shift_dr_out_6[0], shift_dr_out_6[3:1] };	
	
	if(find_str)
	begin
	  if(set_res_str == 0)
	  begin
	    result_string <= res_str;
	    symb_count_reg <= symb_count;
		 set_res_str <= 1;
	  end
	  //led[7] <= 1;
	end
	else
	begin
	  result_string <= {512{1'b0}};
	  symb_count_reg <= symb_count;
	end
	
	
	led[7] <= find_str;
	
	if(end_brute_force)
	begin
	  symb_count_reg <= symb_count;
	end

	if(cdr && (ir_in == 4'b1011))
	begin
		shift_dr_out_5 <= result_string[7:0];
		result_string <= {result_string[7:0], result_string[511:8]};
	end
	else
	if(sdr && (ir_in==4'b1011))
		shift_dr_out_5 <= { shift_dr_out_5[0], shift_dr_out_5[7:1] };
end


//pass or bypass data via tdo reg
always @*
begin
  case(ir_in)
   //4'b0001: tdo = shift_dr_in [0];
   //4'b0010: tdo = shift_dr_out_1[0];
	//4'b0011: tdo = shift_dr_out_2[0];
	//4'b0100: tdo = shift_dr_out_3[0];
	4'b0101: tdo = shift_dr_in_5[0];
	4'b0110: tdo = shift_dr_in_1[0];
	4'b0111: tdo = shift_dr_in_2[0];
	4'b1000: tdo = shift_dr_in_3[0];
	4'b1001: tdo = shift_dr_in_4[0];
	4'b1010: tdo = shift_dr_out_6[0];
	4'b1011: tdo = shift_dr_out_5[0];
	
  default:
   tdo = tdi;
  endcase
end
endmodule