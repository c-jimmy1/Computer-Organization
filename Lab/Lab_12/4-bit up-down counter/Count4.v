// 4-bit up/down counter
module Count4(clk,rst,enable,upDown, count);
   input clk, rst, enable, upDown;
   output wire [3:0] count;

   // insert code here 
   reg [3:0] count_reg;

   always @(posedge clk or posedge rst) begin
       if (rst) begin
           count_reg <= 4'b0000;
       end else if (enable) begin
           if (upDown) begin
               // Count up
               count_reg <= (count_reg == 4'b1111) ? 4'b0000 : count_reg + 1;
           end else begin
               // Count down
               count_reg <= (count_reg == 4'b0000) ? 4'b1111 : count_reg - 1;
           end
       end
   end

   assign count = count_reg;

endmodule  // Count4