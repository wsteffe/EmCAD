<Qucs Schematic 25.1.0>
<Properties>
  <View=-295,-1075,4107,1392,0.342562,0,0>
  <Grid=10,10,1>
  <DataSet=monoliticOnLTCC_net_CST.dat>
  <DataDisplay=monoliticOnLTCC_net_CST.dpl>
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
  <Pac P1 1 150 -50 18 -26 0 1 "1" 1 "50 Ohm" 1 "0 dBm" 0 "1 MHz" 0 "26.85" 0 "true" 0>
  <GND * 1 150 -20 0 0 0 0>
  <Pac P2 1 630 -60 18 -26 0 1 "2" 1 "50 Ohm" 1 "0 dBm" 0 "1 MHz" 0 "26.85" 0 "true" 0>
  <GND * 1 630 -30 0 0 0 0>
  <GND * 1 340 -300 0 0 0 0>
  <.SP SP1 1 170 180 0 70 0 0 "lin" 1 "1 MHz" 1 "30 GHz" 1 "9001" 1 "no" 0 "1" 0 "2" 0 "no" 0 "no" 0>
  <Eqn Eqn1 1 480 250 -32 18 0 0 "dBS11=dB(S[1,1])" 1 "dBS12=dB(S[1,2])" 1 "phS12=phase(S[1,2])" 1 "phS11=phase(S[1,1])" 1 "yes" 0>
  <GND * 1 350 -200 0 -22 1 0>
  <SPfile X2 1 340 -330 -26 -65 0 0 "monolitico_CST.s2p" 1 "rectangular" 0 "linear" 0 "open" 0 "2" 0>
  <SPfile X3 1 350 -110 -26 51 1 0 "monoliticOnLTCC_net_CST.s4p" 1 "rectangular" 0 "linear" 0 "open" 0 "4" 0>
</Components>
<Wires>
  <380 -140 410 -140 "" 0 0 0 "">
  <370 -330 410 -330 "" 0 0 0 "">
  <410 -330 410 -140 "" 0 0 0 "">
  <290 -140 320 -140 "" 0 0 0 "">
  <290 -330 310 -330 "" 0 0 0 "">
  <290 -330 290 -140 "" 0 0 0 "">
  <350 -200 350 -170 "" 0 0 0 "">
  <150 -80 320 -80 "" 0 0 0 "">
  <380 -90 380 -80 "" 0 0 0 "">
  <380 -90 630 -90 "" 0 0 0 "">
</Wires>
<Diagrams>
  <Rect 1750 1153 1434 773 3 #c0c0c0 1 00 1 0 1e+09 3e+10 1 -215.965 50 215.897 1 -1 0.2 1 315 0 225 1 0 0 "" "" "">
	<"monoliticOnLTCC_CST:phS12" #ff0000 0 3 0 0 0>
	<"monoliticOnLTCC:phS12" #ff00ff 0 3 0 0 0>
	<"phS12" #00ff00 0 3 0 0 0>
  </Rect>
  <Rect 50 1190 1458 790 3 #c0c0c0 1 00 1 0 1e+09 3e+10 1 -220 20 220 1 -1 0.2 1 315 0 225 1 0 0 "" "" "">
	<"monoliticOnLTCC_CST:phS11" #ff0000 0 3 0 0 0>
	<"phS11" #00ff00 2 3 0 0 0>
	<"monoliticOnLTCC:phS11" #ff00ff 1 3 0 0 0>
  </Rect>
  <Rect 980 221 1920 1081 3 #c0c0c0 1 00 1 0 1e+09 3e+10 0 -80 5 20 1 -1 0.1 1 315 0 225 1 0 0 "" "" "">
	<"dBS11" #0000ff 0 3 0 0 0>
	<"monoliticOnLTCC_CST:dBS11" #ff0000 2 3 0 0 0>
  </Rect>
</Diagrams>
<Paintings>
</Paintings>
