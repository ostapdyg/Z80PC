module Z80_MMU  #(parameter FLAGS = 4,
				  parameter PA = 12,
				  parameter SIZE = 256)(
					input wire nMREQ,
					input wire nRD,
					input wire nWR,
					inout wire[7:0] ram_data,
					inout wire[7:0] cpu_data,
					input wire[15:0] cpu_addr,
					output reg[PA+8-1:0] ram_addr
				);

	reg [FLAGS+PA-1:0] page_table[SIZE-1:0];
	wire [7:0] page_name;
	wire [7:0] offset;
	wire [19:0] physical_addr;
	// set initial page_table adresses to 16'hff - i (reverse descending order).
	integer i;
	initial 
	begin
        ram_addr = 20'hz;
		for (i=0; i<SIZE; i=i+1) 
		begin
			page_table[i] = 0;
		end
	end

	// ram_address[0] == 0 -> low PA
	// ram_address[0] == 1 -> high PA
	assign page_name = cpu_addr[15:8];
	assign offset = cpu_addr[7:0];
	assign physical_addr[19:0] = {page_table[page_name][PA-1:0], offset};

    assign cpu_data = (~nRD) ? ram_data : 8'bz; // if we are reading, then take data from ram, else set cpu_data to z-state(release the bus)
    assign ram_data = (~nWR & (physical_addr[19:8] ^ 8'hfe >= 2)) ? cpu_data : 8'bz; //if the CPU is writing and it's not changing the page table, forward the data from it's inner bus to the outer bus. Otherwise, let go of the CPU data bus.



	always @(nMREQ or nWR or nRD)
		begin
		if(~nMREQ)
		begin
        	if (physical_addr[19:8] ^ 8'hfe < 2 && ~nWR)
				begin
					if(cpu_addr[0] == 0)
						begin

						$strobe("Page table write: A-%x, D-%x, byte - %b",physical_addr[8:1], cpu_data, physical_addr[0]);
						page_table[physical_addr[8:1]][7:0] = cpu_data[7:0]; //Change the page table entry
						end
					else
						begin
						page_table[physical_addr[8:1]][FLAGS+PA-1:8] = cpu_data[7:0]; //Change the page table entry
						end	
				end
			else
				begin
					ram_addr[PA+8-1:0] = physical_addr; //Set the physical address
				end
		end
		else
			ram_addr[PA+8-1:0] = 20'hz; // $strobe("nMREQ is inactive");
		end
	
endmodule


module z80_test ();
	reg [15:0] virtual_addr;
	reg [7:0] data;
	wire [15:0] physical_addr;
	
	wire [15:0] physical_addr_alias;
	parameter delay = 10;
	Z80_MMU mmu(virtual_addr, data, physical_addr);
	initial 
		begin
			#(delay) virtual_addr = 256'h00ac; data = 8'b10001110;
			#(delay) virtual_addr = 256'hacff; data = 8'b10001110;
		end
		

endmodule