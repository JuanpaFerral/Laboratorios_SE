library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity detector_flanco is

    port (
        clk          : in  std_logic;  
        reset        : in  std_logic; 
        entrada_sync : in  std_logic;
		  pulso        : out std_logic
    );
end entity;

architecture rtl of detector_flanco is
    signal q_prev : std_logic := '0';
	 
begin

	process(clk)
	begin
	 if rising_edge(clk) then 
			if reset = '1' then
				q_prev <= '0';
			else
				q_prev <= entrada_sync;
			end if;
	 end if;
	end process;
	
	pulso <= entrada_sync and (not q_prev);
end architecture;