module Seg7Decode( // convert active-high 4-bit BCD to 7-segment display code, active low outputs
   input [3:0] bin, // binary input
   output reg [6:0] seg7 // LED outputs, active low, assumes common anode LEDs. Not actually registers. 
    );
  
 // insert code here
 
endmodule