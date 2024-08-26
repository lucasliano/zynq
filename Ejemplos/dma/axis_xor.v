`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:     UTN.BA
// Engineer:    Lucas Liaño 
// 
// Create Date: 08/25/2024 11:22:27 PM
// Module Name: axis_xor
// Description: Simple implementation of a sync XOR gate against a given parameter. It implements AXI-Stream interfaces
//              at the I/O
// 
// Dependencies: None
// 
// Revision:
// Revision 0.01 - File Created
// 
//////////////////////////////////////////////////////////////////////////////////

module axis_xor #(
    parameter [31:0] XOR_NUM = 32'hDEADBEEF  // Número para la operación XOR
)(
    input aclk,
    input aresetn,
    
    // AXI Stream Input
    input [31:0] s_axis_tdata,
    input [3:0] s_axis_tkeep,
    input s_axis_tlast,
    input s_axis_tvalid,
    output s_axis_tready,
    // AXI Stream Output
    output [31:0] m_axis_tdata,
    output [3:0] m_axis_tkeep,
    output m_axis_tlast,
    output m_axis_tvalid,
    input m_axis_tready
);

    reg [31:0] xor_result;
    reg valid_reg;
    reg [3:0] keep_reg;
    reg last_reg;

    assign s_axis_tready = m_axis_tready;
    assign m_axis_tdata = xor_result;
    assign m_axis_tvalid = valid_reg;
    assign m_axis_tkeep = keep_reg;
    assign m_axis_tlast = last_reg;

    always @(posedge aclk) begin
        if (!aresetn) begin
            xor_result <= 32'b0;
            valid_reg <= 1'b0;
            keep_reg <= 4'b0;
            last_reg <= 1'b0;
        end else if (s_axis_tvalid && s_axis_tready) begin
            xor_result <= s_axis_tdata ^ XOR_NUM;
            valid_reg <= 1'b1;
            keep_reg <= s_axis_tkeep;
            last_reg <= s_axis_tlast;
        end else if (m_axis_tready) begin
            valid_reg <= 1'b0;
        end
    end
endmodule


