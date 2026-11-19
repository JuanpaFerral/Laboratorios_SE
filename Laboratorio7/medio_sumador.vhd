library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity medio_sumador is
	Port ( A : in std_logic;
			 B : in std_logic;
			 C : out std_logic;
			 S : out std_logic );
			 
end entity;

architecture Behavioral of medio_sumador is

begin 

	C <= A and B;
	S <= A xor B;

end architecture;
