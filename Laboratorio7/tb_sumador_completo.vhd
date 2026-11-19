library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_sumador_completo is
end entity;

architecture Behavioral of tb_sumador_completo is

    signal A_tb    : std_logic := '0';
    signal B_tb    : std_logic := '0';
	 signal Cin_tb  : std_logic := '0';
    signal S_tb    : std_logic;
    signal Cout_tb : std_logic;
	 
begin

	uut: entity work.sumador_completo
		  port map(
						A   => A_tb,
						B   => B_tb,
						Cin => Cin_tb,
						S => S_tb,
						Cout => Cout_tb  );
	
	stim_proc: process
	
		variable entradas : std_logic_vector(2 downto 0);
		variable suma_esp : std_logic_vector(1 downto 0);
		variable total    : unsigned(1 downto 0);
		
	begin
		for i in 0 to 7 loop
        entradas := std_logic_vector(to_unsigned(i, 3));
        A_tb   <= entradas(2);
        B_tb   <= entradas(1);
        Cin_tb <= entradas(0);
		  
		  total    := unsigned'("0" & entradas(2)) + unsigned'("0" & entradas(1)) + unsigned'("0" & entradas(0));
        suma_esp := std_logic_vector(total);
		  
		  wait for 10 ns;

        assert (S_tb = suma_esp(0))
            report "Fallo en salida S | Entradas: A=" & std_logic'image(A_tb) &
                   " B=" & std_logic'image(B_tb) & " Cin=" & std_logic'image(Cin_tb) &
                   " | Obtenido S=" & std_logic'image(S_tb) &
                   " | Esperado S=" & std_logic'image(suma_esp(0))
            severity error;

        assert (Cout_tb = suma_esp(1))
            report "Fallo en salida Cout | Entradas: A=" & std_logic'image(A_tb) &
                   " B=" & std_logic'image(B_tb) & " Cin=" & std_logic'image(Cin_tb) &
                   " | Obtenido Cout=" & std_logic'image(Cout_tb) &
                   " | Esperado Cout=" & std_logic'image(suma_esp(1))
            severity error;
		end loop;

    report "Simulacion de sumador_completo finalizada correctamente (8 casos)." severity note;
    wait;
	 
	end process;
		 
end architecture;