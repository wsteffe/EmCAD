<Qucs Schematic 25.1.0>
<Properties>
  <View=-162,-90,1828,1018,0.706414,0,1>
  <Grid=10,10,1>
  <DataSet=monoliticOnLTCC_CST.dat>
  <DataDisplay=monoliticOnLTCC_CST.dpl>
  <OpenDisplay=0>
  <Script=monoliticOnLTCC.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <.SP SP1 1 210 360 0 70 0 0 "lin" 1 "1 MHz" 1 "30 GHz" 1 "3001" 1 "no" 0 "1" 0 "2" 0 "no" 0 "no" 0>
  <GND * 1 280 130 0 0 0 0>
  <Pac P1 1 190 130 18 -26 0 1 "1" 1 "50 Ohm" 1 "0 dBm" 0 "1 MHz" 0 "26.85" 0 "true" 0>
  <Pac P2 1 380 130 18 -26 0 1 "2" 1 "50 Ohm" 1 "0 dBm" 0 "1 MHz" 0 "26.85" 0 "true" 0>
  <GND * 1 190 160 0 0 0 0>
  <GND * 1 380 160 0 0 0 0>
  <Eqn Eqn1 1 420 470 -32 18 0 0 "dBS11=dB(S[1,1])" 1 "dBS12=dB(S[1,2])" 1 "phS12=phase(S[1,2])" 1 "phS11=phase(S[1,1])" 1 "yes" 0>
  <SPfile X1 1 280 90 -26 -65 0 0 "MonoliticOnLTCC_CST.s2p" 1 "rectangular" 0 "linear" 0 "open" 0 "2" 0>
</Components>
<Wires>
  <280 120 280 130 "" 0 0 0 "">
  <310 100 380 100 "" 0 0 0 "">
  <310 90 310 100 "" 0 0 0 "">
  <190 100 250 100 "" 0 0 0 "">
  <250 90 250 100 "" 0 0 0 "">
</Wires>
<Diagrams>
  <Rect 590 976 899 476 3 #c0c0c0 1 00 1 0 2e+09 3e+10 1 -215.965 100 215.918 1 -1 0.5 1 315 0 225 1 0 0 "" "" "">
	<"monoliticOnLTCC_net:phS12" #0000ff 1 3 0 0 0>
	<"phS12" #ff0000 1 3 0 0 0>
  </Rect>
  <Rect 570 396 863 446 3 #c0c0c0 1 00 1 0 2e+09 3e+10 0 -50 5 0 1 -1 0.2 1 315 0 225 1 0 0 "" "" "">
	<"dBS11" #ff00ff 0 3 0 0 0>
  </Rect>
</Diagrams>
<Paintings>
</Paintings>
