-- Copyright (C) 2020  Intel Corporation. All rights reserved.
-- Your use of Intel Corporation's design tools, logic functions 
-- and other software and tools, and any partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Intel Program License 
-- Subscription Agreement, the Intel Quartus Prime License Agreement,
-- the Intel FPGA IP License Agreement, or other applicable license
-- agreement, including, without limitation, that your use is for
-- the sole purpose of programming logic devices manufactured by
-- Intel and sold by Intel or its authorized distributors.  Please
-- refer to the applicable agreement for further details, at
-- https://fpgasoftware.intel.com/eula.

-- PROGRAM		"Quartus Prime"
-- VERSION		"Version 20.1.1 Build 720 11/11/2020 SJ Lite Edition"
-- CREATED		"Thu Nov 19 15:44:29 2026"

LIBRARY ieee;
USE ieee.std_logic_1164.all; 

LIBRARY work;

ENTITY sumador4_bdf IS 
	PORT
	(
		Cin :  IN  STD_LOGIC;
		A :  IN  STD_LOGIC_VECTOR(3 DOWNTO 0);
		B :  IN  STD_LOGIC_VECTOR(3 DOWNTO 0);
		Cout :  OUT  STD_LOGIC;
		S :  OUT  STD_LOGIC_VECTOR(3 DOWNTO 0)
	);
END sumador4_bdf;

ARCHITECTURE bdf_type OF sumador4_bdf IS 

COMPONENT sumador_completo
	PORT(A : IN STD_LOGIC;
		 B : IN STD_LOGIC;
		 Cin : IN STD_LOGIC;
		 Cout : OUT STD_LOGIC;
		 S : OUT STD_LOGIC
	);
END COMPONENT;

SIGNAL	SYNTHESIZED_WIRE_0 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_1 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_2 :  STD_LOGIC;


BEGIN 



b2v_inst : sumador_completo
PORT MAP(A => A(3),
		 B => B(3),
		 Cin => SYNTHESIZED_WIRE_0,
		 Cout => Cout,
		 S => S(3));


b2v_inst1 : sumador_completo
PORT MAP(A => A(0),
		 B => B(0),
		 Cin => Cin,
		 Cout => SYNTHESIZED_WIRE_2,
		 S => S(0));


b2v_inst2 : sumador_completo
PORT MAP(A => A(2),
		 B => B(2),
		 Cin => SYNTHESIZED_WIRE_1,
		 Cout => SYNTHESIZED_WIRE_0,
		 S => S(2));


b2v_inst3 : sumador_completo
PORT MAP(A => A(1),
		 B => B(1),
		 Cin => SYNTHESIZED_WIRE_2,
		 Cout => SYNTHESIZED_WIRE_1,
		 S => S(1));


END bdf_type;