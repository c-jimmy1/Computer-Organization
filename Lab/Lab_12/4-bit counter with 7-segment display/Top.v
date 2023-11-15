module Top(clk, rst, enable, upDown, count, seg7);
   // count[3:0] output included for convenience in debug and testbenches
   input clk, rst, enable, upDown;
   output [3:0] count;
   output [6:0] seg7;

   wire clkCounter;  // divided clock for Counter module

   // Instantiate the clock divider
   ClkDiv clk_div_inst(.clk(clk), .clkOut(clkCounter));

   // Instantiate the up-down counter
   Count4 counter_inst(
      .clk(clkCounter),
      .rst(rst),
      .enable(enable),
      .upDown(upDown),
      .count(count)
   );

   // Instantiate the 7-segment LED decoder
   Seg7decode seg7_inst(
      .bin(count),
      .seg7(seg7)
   );

endmodule