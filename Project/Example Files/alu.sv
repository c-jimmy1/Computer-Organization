// Adopted from https://www.fpga4student.com/2017/06/Verilog-code-for-ALU.html
`timescale 1ns / 1ps

module alu(
           input [15:0] A,B,  // ALU 16-bit Inputs
           input [1:0] ALU_Sel,// ALU Selection
           output [15:0] ALU_Out // ALU 16-bit Output
    );
    reg [15:0] ALU_Result;
    wire [15:0] tmp;
    assign ALU_Out = ALU_Result; // ALU out
    always @(*)
    begin
        case(ALU_Sel)
        2'b01: // Addition
           ALU_Result = A + B ;
        2'b10: // Subtraction
           ALU_Result = A - B ;
        3'b11: // Clear
           ALU_Result = 0;
        default: ALU_Result = A;
        endcase
    end

endmodule
