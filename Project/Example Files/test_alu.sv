// Adopted from https://www.fpga4student.com/2017/06/Verilog-code-for-ALU.html
`timescale 1ns / 1ps

module test_alu;
//Inputs
reg[15:0] A,B;
reg[1:0] ALU_Sel;

//Outputs
wire[15:0] ALU_Out;
// Verilog code for ALU
integer i;
alu u0(
            A,B,  // ALU 16-bit Inputs
            ALU_Sel,// ALU Selection
            ALU_Out // ALU 16-bit Output
     );
    initial begin
    $dumpfile("dump.vcd");
    $dumpvars;
    A = 'hFA;
    B = 'h02;
    ALU_Sel = 2'h0;
      
      for (i=0;i<4;i=i+1)
      begin
       ALU_Sel = ALU_Sel + 2'h1;
       #10;
      end;
    end
endmodule
