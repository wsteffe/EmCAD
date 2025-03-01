<Qucs Schematic 25.1.0>
<Properties>
  <View=96,-440,3135,1433,0.431863,0,0>
  <Grid=10,10,1>
  <DataSet=monoliticOnLTCC_net.dat>
  <DataDisplay=monoliticOnLTCC_net.dpl>
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
  <GND * 1 340 30 0 0 0 0>
  <SPfile X2 1 340 -330 -26 -65 0 0 "monolitico.s2p" 1 "rectangular" 0 "linear" 0 "open" 0 "2" 0>
  <GND * 1 340 -300 0 0 0 0>
  <SPfile X3 1 340 -150 -26 -95 0 0 "MonoliticoOnLTCC_net.s4p" 1 "rectangular" 0 "linear" 0 "open" 0 "4" 0>
  <Eqn Eqn1 1 480 260 -32 18 0 0 "dBS11=dB(S[1,1])" 1 "dBS12=dB(S[1,2])" 1 "phS12=phase(S[1,2])" 1 "phS11=phase(S[1,1])" 1 "yes" 0>
  <.SP SP1 1 170 180 0 70 0 0 "lin" 1 "1 MHz" 1 "30 GHz" 1 "9001" 1 "no" 0 "1" 0 "2" 0 "no" 0 "no" 0>
</Components>
<Wires>
  <150 -80 210 -80 "" 0 0 0 "">
  <210 -90 210 -80 "" 0 0 0 "">
  <340 -90 340 30 "" 0 0 0 "">
  <370 -120 370 -90 "" 0 0 0 "">
  <370 -90 630 -90 "" 0 0 0 "">
  <310 -120 310 -90 "" 0 0 0 "">
  <210 -90 310 -90 "" 0 0 0 "">
  <370 -180 450 -180 "" 0 0 0 "">
  <370 -330 450 -330 "" 0 0 0 "">
  <450 -330 450 -180 "" 0 0 0 "">
  <200 -180 310 -180 "" 0 0 0 "">
  <200 -330 310 -330 "" 0 0 0 "">
  <200 -330 200 -180 "" 0 0 0 "">
</Wires>
<Diagrams>
  <Rect 590 1386 1171 626 3 #c0c0c0 1 00 1 0 0.2 1 1 -0.1 0.5 1.1 1 -0.1 0.5 1.1 315 0 225 1 0 0 "" "" "">
	<"phS11" #0000ff 1 3 0 0 0>
	<"monoliticOnLTCC:phS11" #ff0000 1 3 0 0 0>
  </Rect>
  <Rect 1960 1361 1133 601 3 #c0c0c0 1 00 1 0 0.2 1 1 -0.1 0.5 1.1 1 -0.1 0.5 1.1 315 0 225 1 0 0 "" "" "">
	<"monoliticOnLTCC:phS12" #0000ff 1 3 0 0 0>
	<"phS12" #ff0000 1 3 0 0 0>
  </Rect>
  <Rect 910 642 1705 1002 3 #c0c0c0 1 00 1 0 1e+09 3e+10 0 -80 5 20 1 -1 0.2 1 315 0 225 1 0 0 "" "" "">
	<"dBS11" #0000ff 0 3 0 0 0>
	<"monoliticOnLTCC:dBS11" #ff0000 1 3 0 0 0>
  </Rect>
</Diagrams>
<Paintings>
</Paintings>
