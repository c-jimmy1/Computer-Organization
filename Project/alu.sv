`timescale 1ns / 1ps

module alu(input [15:0] A,B,  // ALU 16-bit Inputs
           input [3:0] ALU_Sel,// ALU Selection
           output [15:0] ALU_Out // ALU 16-bit Output
         );
   reg [15:0] ALU_Result;
   wire [15:0] tmp;
   assign ALU_Out = ALU_Result; // ALU out
   
   always @(*) 
   begin
      case (ALU_Sel) 
      
      endcase
   end
endmodule
