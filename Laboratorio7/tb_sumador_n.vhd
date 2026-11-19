library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_sumador_n is
end entity;

architecture Behavioral of tb_sumador_n is

	 constant N : positive := 4;

    signal A_tb    : std_logic_vector(N - 1 downto 0) := (others => '0');
    signal B_tb    : std_logic_vector(N - 1 downto 0) := (others => '0');
	 signal Cin_tb  : std_logic := '0';
    signal S_tb    : std_logic_vector(N - 1 downto 0);
    signal Cout_tb : std_logic;
	 
begin

	---uut: entity work.sumador_n
	---	  generic map
	---	  (
	---			N => N
	---	  )
	
	---	  port map(
	---					A   => A_tb,
	---					B   => B_tb,
	---					Cin => Cin_tb,
	---					S => S_tb,
	---					Cout => Cout_tb  ); 
	uut: entity work.sumador4_bdf
        port map (
            A    => A_tb,
            B    => B_tb,
            Cin  => Cin_tb,
            S    => S_tb,
            Cout => Cout_tb );
	
	stim_proc: process
	
		variable resultado_esperado : integer;
      variable esperado_vec       : std_logic_vector(N downto 0);
      variable obtenido_vec       : std_logic_vector(N downto 0);
      variable total_casos        : integer := 0;
		
	begin
		for a_int in 0 to (2**N) - 1 loop
            for b_int in 0 to (2**N) - 1 loop
                for cin_int in 0 to 1 loop
					 
							A_tb <= std_logic_vector(to_unsigned(a_int, N));
							B_tb <= std_logic_vector(to_unsigned(b_int, N));
							
						   if cin_int = 0 then
                        Cin_tb <= '0';
                     else
                        Cin_tb <= '1';
                     end if;
							
							wait for 10 ns;
							
							resultado_esperado := a_int + b_int + cin_int;
                     esperado_vec       := std_logic_vector(to_unsigned(resultado_esperado, N + 1));
                     obtenido_vec       := Cout_tb & S_tb;
							
							assert (obtenido_vec = esperado_vec)
                        report "Fallo en caso #" & integer'image(total_casos + 1) &
                               " | Operandos: A=" & integer'image(a_int) &
                               " B=" & integer'image(b_int) &
                               " Cin=" & integer'image(cin_int) &
                               " | Obtenido (Cout&S)=" & integer'image(to_integer(unsigned(obtenido_vec))) &
                               " | Esperado=" & integer'image(resultado_esperado)
                        severity error;

                    total_casos := total_casos + 1;
					end loop;
				end loop;
			end loop;
			
			report "VERIFICACION EXITOSA: Se completaron " & integer'image(total_casos) &
               " casos sin errores para N=" & integer'image(N)
            severity note;
			wait;
	 
	end process;
		 
end architecture;     