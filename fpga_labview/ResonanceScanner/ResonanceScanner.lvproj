<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="12008004">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">true</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="SubVIs" Type="Folder" URL="../SubVIs">
			<Property Name="NI.DISK" Type="Bool">true</Property>
		</Item>
		<Item Name="DetectPulseIntervals.vi" Type="VI" URL="../../Common Host VIs/DetectPulseIntervals.vi"/>
		<Item Name="FPGA Target" Type="FPGA Target">
			<Property Name="AutoRun" Type="Bool">false</Property>
			<Property Name="configString.guid" Type="Str">{059E4B14-88FE-4D83-A68D-D044D78DABCF}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=bool{07169EDD-07AE-44E5-B31A-8F897BFC26B3}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=bool{083AA03D-0D34-47EF-9BC1-8C944B14D2CC}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16{09BC2919-DADF-47D6-8A54-2EEB64346FB8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=bool{251B0674-6FD8-4BBE-A7C7-0938E8B45849}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16{2D78DFA7-362E-4C80-A916-D97D3A069FB8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=bool{31AB561C-FFEA-4C03-AE81-45435E23A54A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=bool{31D220AC-2743-4E8A-A2F1-3CA7704D7673}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=bool{346C2B77-65E0-4C38-B075-E94E9A442EFE}ResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{35EC8E5E-F58D-4D69-97C9-1FAB856A706A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16{38A206DF-3763-4660-AE58-3A1E9221B8F1}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{39950353-3128-42D1-9CDC-61997DA35418}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16{39A12BEA-5368-4BB4-98E8-5559E07C9E74}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32{3A96F3E5-B6B5-412B-B1C8-0FDB7CF45049}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=bool{4106B0CB-1151-4A22-85E9-0B95AA3AC17C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16{459F9D74-5655-43B8-AC25-C2DA57E54F40}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=bool{47082E8D-E476-45C6-950A-23BCCC63BCC0}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8{4F28F572-BB2E-44A8-ADBC-EE7C83B22EF6}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=bool{4F63B79B-75FD-49CE-91E9-FFE9D85A281D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=bool{549D72D5-A63D-42BA-A51C-CCFB6516D40C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=bool{5772F6AE-B96B-4D58-B490-B196E43795EB}NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=bool{5814C834-0EF7-481A-A5AE-43F1C2319C46}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=bool{58EBBC03-D469-47F3-AE9C-8A06EEA78C28}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=bool{5BF07CC5-1944-4440-B50F-B3991EDBB20B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=bool{619FE4DA-6660-465D-A1E9-3FE7D2522486}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16{67C0686C-7710-46FB-ACE5-4EF8A4C3D442}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16{6CE8D09A-52D3-4D89-89B8-14A0AFFCC65C}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{70D358EB-0B4B-40E0-BF7C-C10B6CFB5C52}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16{74A8DC07-8A69-4F6E-AD03-98F9BF18AC77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16{76A096C4-C54C-4822-9ACA-439BD134E65E}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=bool{7A15B218-6947-42A8-97AA-F1137906021B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16{7BCE8608-B87E-4C6D-93D5-000D317E7AA8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16{7CC27B7E-5B7F-49D2-99F4-AC26D2620C05}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16{7EF48D8A-1162-479C-9A48-04F0D5D6AFBD}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32{7F40C34B-007E-4544-934E-1C3B97EE55EB}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=bool{81049F1A-582A-465B-ACDE-DB4332124514}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=bool{8369A44D-B440-444E-AB26-86B545110EF2}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16{83C7C6DB-B0EF-4196-8464-2C58C524A6A4}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=bool{8DC9D4EC-7AA6-447B-A3CD-5CDA0E44BE12}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8{963909BA-5DCF-4D97-857A-450B8BACDC4A}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=bool{9F2CFCFF-DE82-427A-803C-3647D5444383}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool{9F436645-C048-4925-BD00-0E3AD9A6C808}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16{A542AAE4-16D6-4F09-AFD0-4D67EFCD22B6}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=bool{AB50E9C5-6490-47A3-B625-EC6D1143DD07}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16{AC2C9003-5340-43DC-8301-B31AA0709F74}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=bool{AF2B5481-152F-44AC-8B5D-992A5CA04752}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16{B28EC9CC-C123-4FC3-9DAC-E68CE7C4FE0E}Multiplier=1.000000;Divisor=4.000000{B4233A0B-431B-4E58-A4DA-40987CFB4141}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=bool{B468824C-28E9-4D74-8E62-8879CA75640B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16{B64EA97B-7C27-48B4-AD56-CDC51C9603FC}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=bool{B77D96A7-3EDB-4BB7-B5D8-44BB5D533568}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=bool{B8822C26-F841-4056-8CFB-894B5C5BF477}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=bool{B8B3D13C-F54E-4349-A5B1-A78C4151576D}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=bool{B8D18856-BD94-451A-BA55-6CBD6646F171}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=bool{C1DD7BF0-6A1E-4C73-A871-1428CB5F8FFA}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=bool{C6F16304-3899-48C2-88BF-8338B22C7D08}ResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{C7027B38-BA2C-4A7A-867E-31F526CEC5D8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=bool{C9B9582B-FD64-446D-A112-B7ADBD7ECB62}ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{CFFC33B9-2530-4A46-AF8F-CF91E059B6E3}"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"{D4007612-93E7-4F24-8BF7-055EDA438857}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=bool{D476AA2C-1E6A-4981-9C40-37061CAD7CE8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=bool{D4FCC07B-E776-4BDA-ADB8-042D662B05FB}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=bool{D57DFE40-3C0D-424A-96DB-8F4B76E5181E}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=bool{D75C180A-7216-45AD-8B38-C6702DF1BE36}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16{E29B5FF1-BB25-4515-81C9-09267F585B77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=bool{E580D444-134F-4148-B7A4-665A022C06DE}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=bool{E7A76451-50BC-4789-A331-3447EA58AF7F}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16{EBD4042E-911B-4524-8CA2-FC053981E6C6}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=bool{EC07F106-2868-4BBF-A650-59DBEB62B855}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=bool{ED2A02E6-8B51-4F86-87DC-A026793EB745}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=bool{EE7BDDBE-7382-4C23-A4F5-71DBAFDE6DE4}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=bool{F830D38C-B78A-4DEA-BFB7-1F41DB00204F}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=bool{F94814D9-2569-44A7-95EE-6F9AD43FF8DA}67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
{FC0A9E4C-5E7A-4CF8-8384-A4346539B90D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGA</Property>
			<Property Name="configString.name" Type="Str">10MHzMultiplier=1.000000;Divisor=4.000000200 MHz ClockResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E40 MHz Onboard ClockResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EADC Error ANumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=boolADC Error BNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=boolAI A0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16AI A1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16AI A2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16AI A3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16AI A4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16AI A5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16AI A6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16AI A7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16AI B10NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16AI B11NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16AI B12NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16AI B13NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16AI B14NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16AI B15NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16AI B8NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16AI B9NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16Clock100 PLL UnlockedNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=boolD0 0ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=boolD0 1ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=boolD0 2ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=boolD0 3ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=boolD0 4ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=boolD0 5ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=boolD0 6ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=boolD0 7ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=boolDevice TemperatureNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16DI 0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=boolDI 1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=boolDI 2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=boolDI 3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=boolDI 4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=boolDI 5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=boolDI 6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=boolDI 7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=boolDigital Output EnableArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=boolEEPROM Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=boolExpected IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32Force InitializationArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=boolInitialization DoneNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=boolInserted IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32InterloopFIFO"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"IO Module Clock 0ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EIO Module IO EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=boolIO Module Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=boolIO Module Power GoodNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolIO Module PresentNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=boolIO Module67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
LineClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=boolPixelClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=boolPll UnlockedNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=boolPXI_Clk10NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=boolPXI_StarNumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=boolPXI_Trig3ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig4ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig5ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig6ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig7ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=boolPXIe_DStarBNumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=boolPXIe_DStarCArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=boolPXIe_Sync100NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGASample Clock SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8SPI AddressArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16SPI Device SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8SPI IdleNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=boolSPI Write DataArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16SPI WriteArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=boolToHostCh1FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"ToHostCh2FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"TriggerInArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool</Property>
			<Property Name="NI.FPGA.79XXR.ConfiguredIOModule" Type="Str">IOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA</Property>
			<Property Name="NI.LV.FPGA.CLIPDeclarations[0].Category" Type="Str">FlexRIO-IOModule</Property>
			<Property Name="NI.LV.FPGA.CLIPDeclarations[0].InvalidPath" Type="Str"></Property>
			<Property Name="NI.LV.FPGA.CLIPDeclarations[0].Name" Type="Str">NI 5751</Property>
			<Property Name="NI.LV.FPGA.CLIPDeclarationsArraySize" Type="Int">1</Property>
			<Property Name="NI.LV.FPGA.CLIPDeclarationSet" Type="Xml">
<CLIPDeclarationSet>
   <CLIPDeclarationCategory name="FlexRIO-IOModule">
      <CLIPDeclaration name="NI 5751">
         <CLIPVersion>1.1.0</CLIPVersion>
         <CompatibleCLIPSocketList>
            <Socket>FlexRIO-IOModule</Socket>
         </CompatibleCLIPSocketList>
         <CompatibleIOModuleList>
            <IOModule>IOModuleID:0x109374F2</IOModule>
         </CompatibleIOModuleList>
         <DeclarationPaths>
            <Absolute>C:\Program Files (x86)\National Instruments\Shared\FlexRIO\IO Modules\NI 5751\NI5751\1.0.0\NI5751FamClk.xml</Absolute>
            <MD5>40eb31d638a681f3bf7d5fac39d8634e</MD5>
            <RelativeToLabVIEW>C:\Program Files (x86)\National Instruments\Shared\FlexRIO\IO Modules\NI 5751\NI5751\1.0.0\NI5751FamClk.xml</RelativeToLabVIEW>
            <RelativeToNiPubDocs>C:\Program Files (x86)\National Instruments\Shared\FlexRIO\IO Modules\NI 5751\NI5751\1.0.0\NI5751FamClk.xml</RelativeToNiPubDocs>
            <RelativeToNiSharedDir>FlexRIO\IO Modules\NI 5751\NI5751\1.0.0\NI5751FamClk.xml</RelativeToNiSharedDir>
            <RelativeToProject>..\..\..\Program Files (x86)\National Instruments\Shared\FlexRIO\IO Modules\NI 5751\NI5751\1.0.0\NI5751FamClk.xml</RelativeToProject>
            <Valid>true</Valid>
         </DeclarationPaths>
         <Description>This CLIP provides access to sixteen analog input channels, eight digital input lines, and eight digital output lines.  This CLIP also contains a SPI interface to program the ADC registers.  

In this CLIP, each sample clock cycle generates a sample from the analog input channels.  Three clock sources are available and are selectable using the Sample Clock Select control.  The default clock source is the 50 Mhz onboard oscillator.  Other clock sources available are DStarA through IoModSyncClock and an external clock through the front panel SMB connector.  Only external sample clock rates from 30 MHz to 50Mhz are supported with this CLIP.  Each 14-bit sample is output to LabView as an I16.  The 14-bit data is left justified and padded with 2 zeros in the LSBs.  The data is clocked out of the CLIP on IoModClock0.  

For more information on the CLIP I/O refer to the NI 5751 FlexRIO CLIP Node Wire Descriptions section of the User Guide.</Description>
         <FormatVersion>1.1</FormatVersion>
         <HDLName>NI5751FamClk</HDLName>
         <ImplementationList>
            <Path>NI5751FamClk.vhd</Path>
            <Path>Ni5751Base.vhd</Path>
            <Path>Ni5751Top.ngc</Path>
            <Path>NI5751FamClk.ucf</Path>
            <Path>PkgNi5751.vhd</Path>
            <VerifiedImplementationList>
               <Path name="PkgNi5751.vhd">
                  <MD5>9207c7e191aec536f8c72983cb67bc3a</MD5>
                  <SimulationFileList>
                     <SimulationModelType>Same as synthesis</SimulationModelType>
                  </SimulationFileList>
               </Path>
               <Path name="NI5751FamClk.vhd">
                  <MD5>a80b7a2f59294e4f38713f732f2478fc</MD5>
                  <SimulationFileList>
                     <SimulationModelType>Same as synthesis</SimulationModelType>
                  </SimulationFileList>
               </Path>
               <Path name="Ni5751Base.vhd">
                  <MD5>52de049f325cfa1a32f3ebdd679fcbf4</MD5>
                  <SimulationFileList>
                     <SimulationModelType>Same as synthesis</SimulationModelType>
                  </SimulationFileList>
               </Path>
               <Path name="Ni5751Top.ngc">
                  <MD5>b23f4a83f40a50f4d63de7f83d2bcb6c</MD5>
                  <SimulationFileList>
                     <SimulationModelType>Exclude from simulation model</SimulationModelType>
                  </SimulationFileList>
               </Path>
               <Path name="NI5751FamClk.ucf">
                  <MD5>ec8e84e96d2a10a3874661cdf6ad609a</MD5>
                  <SimulationFileList>
                     <SimulationModelType>Same as synthesis</SimulationModelType>
                  </SimulationFileList>
               </Path>
            </VerifiedImplementationList>
         </ImplementationList>
         <InterfaceList>
            <Interface name="Fabric">
               <InterfaceType>Fabric</InterfaceType>
               <SignalList>
                  <Signal name="aResetSl">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>aResetSl</HDLName>
                     <SignalType>reset</SignalType>
                  </Signal>
               </SignalList>
            </Interface>
            <Interface name="5751 CLIP">
               <InterfaceType>LabVIEW</InterfaceType>
               <SignalList>
                  <Signal name="Clock40">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <FreqInHertz>
                        <Max>45M</Max>
                        <Min>35M</Min>
                     </FreqInHertz>
                     <HDLName>Clock40</HDLName>
                     <SignalType>clock</SignalType>
                  </Signal>
                  <Signal name="Clock200">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <FreqInHertz>
                        <Max>205M</Max>
                        <Min>195M</Min>
                     </FreqInHertz>
                     <HDLName>Clock200</HDLName>
                     <SignalType>clock</SignalType>
                  </Signal>
                  <Signal name="AI A0">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData0</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI A1">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData1</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI A2">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData2</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI A3">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData3</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI A4">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData4</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI A5">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData5</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI A6">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData6</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI A7">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData7</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI B8">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData8</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI B9">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData9</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI B10">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData10</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI B11">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData11</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI B12">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData12</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI B13">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData13</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI B14">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData14</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="AI B15">
                     <DataType>
                        <I16></I16>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sData15</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="ADC Error A">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sAdcErrorA</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="ADC Error B">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>sAdcErrorB</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="Pll Unlocked">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>cPllUnlockedStky</HDLName>
                     <RequiredClockDomain>Clock40</RequiredClockDomain>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="Force Initialization">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>cForceInit</HDLName>
                     <RequiredClockDomain>Clock40</RequiredClockDomain>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="Initialization Done">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>cInitDone</HDLName>
                     <RequiredClockDomain>Clock40</RequiredClockDomain>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="SPI Idle">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>cUserSpiIdle</HDLName>
                     <RequiredClockDomain>Clock40</RequiredClockDomain>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="SPI Device Select">
                     <DataType>
                        <U8></U8>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>cUserSpiDevice</HDLName>
                     <RequiredClockDomain>Clock40</RequiredClockDomain>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="SPI Address">
                     <DataType>
                        <U16></U16>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>cUserSpiAddr</HDLName>
                     <RequiredClockDomain>Clock40</RequiredClockDomain>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="SPI Write Data">
                     <DataType>
                        <U16></U16>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>cUserSpiWrData</HDLName>
                     <RequiredClockDomain>Clock40</RequiredClockDomain>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="SPI Write">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>cUserSpiWriteEn</HDLName>
                     <RequiredClockDomain>Clock40</RequiredClockDomain>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="Sample Clock Select">
                     <DataType>
                        <U8></U8>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>cClkSel</HDLName>
                     <RequiredClockDomain>Clock40</RequiredClockDomain>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="DI 0">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromClip</Direction>
                     <HDLName>aDI0</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="DI 1">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromClip</Direction>
                     <HDLName>aDI1</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="DI 2">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromClip</Direction>
                     <HDLName>aDI2</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="DI 3">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromClip</Direction>
                     <HDLName>aDI3</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="DI 4">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromClip</Direction>
                     <HDLName>aDI4</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="DI 5">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromClip</Direction>
                     <HDLName>aDI5</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="DI 6">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromClip</Direction>
                     <HDLName>aDI6</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="DI 7">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromClip</Direction>
                     <HDLName>aDI7</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="D0 0">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToClip</Direction>
                     <HDLName>aDO0</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="D0 1">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToClip</Direction>
                     <HDLName>aDO1</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="D0 2">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToClip</Direction>
                     <HDLName>aDO2</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="D0 3">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToClip</Direction>
                     <HDLName>aDO3</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="D0 4">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToClip</Direction>
                     <HDLName>aDO4</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="D0 5">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToClip</Direction>
                     <HDLName>aDO5</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="D0 6">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToClip</Direction>
                     <HDLName>aDO6</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="D0 7">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToClip</Direction>
                     <HDLName>aDO7</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
                  <Signal name="Digital Output Enable">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>aDigitalOutputEnable</HDLName>
                     <SignalType>data</SignalType>
                  </Signal>
               </SignalList>
            </Interface>
            <Interface name="Socket">
               <InterfaceType>Socket</InterfaceType>
               <SignalList>
                  <Signal name="aUserGpio">
                     <DataType>
                        <Array>
                           <Boolean></Boolean>
                           <Size>66</Size>
                        </Array>
                     </DataType>
                     <Direction>Bidirectional</Direction>
                     <HDLName>aUserGpio</HDLName>
                  </Signal>
                  <Signal name="aUserGpio_n">
                     <DataType>
                        <Array>
                           <Boolean></Boolean>
                           <Size>66</Size>
                        </Array>
                     </DataType>
                     <Direction>Bidirectional</Direction>
                     <HDLName>aUserGpio_n</HDLName>
                  </Signal>
                  <Signal name="rIoModGpioEn">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>rIoModGpioEn</HDLName>
                  </Signal>
                  <Signal name="UserGClkLvds">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>UserGClkLvds</HDLName>
                  </Signal>
                  <Signal name="UserGClkLvds_n">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>UserGClkLvds_n</HDLName>
                  </Signal>
                  <Signal name="UserGClkLvttl">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>UserGClkLvttl</HDLName>
                  </Signal>
                  <Signal name="IoModClipClock0">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>IoModClipClock0</HDLName>
                  </Signal>
                  <Signal name="IoModClipClock1">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>IoModClipClock1</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaReqI2cBus">
                     <CLIPSignal>rLvFpgaReqI2cBus</CLIPSignal>
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>rLvFpgaReqI2cBus</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaAckI2cBus">
                     <CLIPSignal>rLvFpgaAckI2cBus</CLIPSignal>
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>rLvFpgaAckI2cBus</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaI2cGo">
                     <CLIPSignal>rLvFpgaI2cGo</CLIPSignal>
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>rLvFpgaI2cGo</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaI2cStart">
                     <CLIPSignal>rLvFpgaI2cStart</CLIPSignal>
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>rLvFpgaI2cStart</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaI2cStop">
                     <CLIPSignal>rLvFpgaI2cStop</CLIPSignal>
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>rLvFpgaI2cStop</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaI2cRd">
                     <CLIPSignal>rLvFpgaI2cRd</CLIPSignal>
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>rLvFpgaI2cRd</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaI2cAck">
                     <CLIPSignal>rLvFpgaI2cAck</CLIPSignal>
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>rLvFpgaI2cAck</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaI2cDone">
                     <CLIPSignal>rLvFpgaI2cDone</CLIPSignal>
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>rLvFpgaI2cDone</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaI2cWtData">
                     <CLIPSignal>rLvFpgaI2cWtData</CLIPSignal>
                     <DataType>
                        <Array>
                           <Boolean></Boolean>
                           <Size>8</Size>
                        </Array>
                     </DataType>
                     <Direction>FromCLIP</Direction>
                     <HDLName>rLvFpgaI2cWtData</HDLName>
                  </Signal>
                  <Signal name="rLvFpgaI2cRdData">
                     <CLIPSignal>rLvFpgaI2cRdData</CLIPSignal>
                     <DataType>
                        <Array>
                           <Boolean></Boolean>
                           <Size>8</Size>
                        </Array>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>rLvFpgaI2cRdData</HDLName>
                  </Signal>
                  <Signal name="rClkToSocket">
                     <DataType>
                        <Boolean></Boolean>
                     </DataType>
                     <Direction>ToCLIP</Direction>
                     <HDLName>rClkToSocket</HDLName>
                  </Signal>
               </SignalList>
            </Interface>
         </InterfaceList>
      </CLIPDeclaration>
   </CLIPDeclarationCategory>
</CLIPDeclarationSet></Property>
			<Property Name="NI.LV.FPGA.CompileConfigString" Type="Str">PXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGA</Property>
			<Property Name="NI.LV.FPGA.Version" Type="Int">5</Property>
			<Property Name="NI.SortType" Type="Int">3</Property>
			<Property Name="Resource Name" Type="Str">RIO0</Property>
			<Property Name="Target Class" Type="Str">PXIe-7962R</Property>
			<Property Name="Top-Level Timing Source" Type="Str">40 MHz Onboard Clock</Property>
			<Property Name="Top-Level Timing Source Is Default" Type="Bool">true</Property>
			<Item Name="Board IO" Type="Folder">
				<Item Name="Clock100 PLL Unlocked" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/Board IO/Clock100 PLL Unlocked</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{963909BA-5DCF-4D97-857A-450B8BACDC4A}</Property>
				</Item>
				<Item Name="Device Temperature" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/Board IO/Device Temperature</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{D75C180A-7216-45AD-8B38-C6702DF1BE36}</Property>
				</Item>
			</Item>
			<Item Name="IO Module Status" Type="Folder">
				<Item Name="EEPROM Power Enabled" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module Status/EEPROM Power Enabled</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{4F63B79B-75FD-49CE-91E9-FFE9D85A281D}</Property>
				</Item>
				<Item Name="Expected IO Module ID" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module Status/Expected IO Module ID</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{7EF48D8A-1162-479C-9A48-04F0D5D6AFBD}</Property>
				</Item>
				<Item Name="Inserted IO Module ID" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module Status/Inserted IO Module ID</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{39A12BEA-5368-4BB4-98E8-5559E07C9E74}</Property>
				</Item>
				<Item Name="IO Module IO Enabled" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module Status/IO Module IO Enabled</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{76A096C4-C54C-4822-9ACA-439BD134E65E}</Property>
				</Item>
				<Item Name="IO Module Power Enabled" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module Status/IO Module Power Enabled</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{B4233A0B-431B-4E58-A4DA-40987CFB4141}</Property>
				</Item>
				<Item Name="IO Module Power Good" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module Status/IO Module Power Good</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{FC0A9E4C-5E7A-4CF8-8384-A4346539B90D}</Property>
				</Item>
				<Item Name="IO Module Present" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module Status/IO Module Present</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{EBD4042E-911B-4524-8CA2-FC053981E6C6}</Property>
				</Item>
			</Item>
			<Item Name="PXI" Type="Folder">
				<Item Name="LineClock" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="ArbitrationForOutputEnable">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputEnable">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Trig2</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{EE7BDDBE-7382-4C23-A4F5-71DBAFDE6DE4}</Property>
				</Item>
				<Item Name="PixelClock" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="ArbitrationForOutputEnable">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputEnable">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Trig1</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{07169EDD-07AE-44E5-B31A-8F897BFC26B3}</Property>
				</Item>
				<Item Name="PXI_Clk10" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Clk10</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{D57DFE40-3C0D-424A-96DB-8F4B76E5181E}</Property>
				</Item>
				<Item Name="PXI_Star" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Star</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{F830D38C-B78A-4DEA-BFB7-1F41DB00204F}</Property>
				</Item>
				<Item Name="PXI_Trig3" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="ArbitrationForOutputEnable">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputEnable">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Trig3</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{D4007612-93E7-4F24-8BF7-055EDA438857}</Property>
				</Item>
				<Item Name="PXI_Trig4" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="ArbitrationForOutputEnable">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputEnable">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Trig4</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{C7027B38-BA2C-4A7A-867E-31F526CEC5D8}</Property>
				</Item>
				<Item Name="PXI_Trig5" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="ArbitrationForOutputEnable">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputEnable">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Trig5</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{E580D444-134F-4148-B7A4-665A022C06DE}</Property>
				</Item>
				<Item Name="PXI_Trig6" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="ArbitrationForOutputEnable">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputEnable">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Trig6</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{2D78DFA7-362E-4C80-A916-D97D3A069FB8}</Property>
				</Item>
				<Item Name="PXI_Trig7" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="ArbitrationForOutputEnable">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputEnable">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Trig7</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{B64EA97B-7C27-48B4-AD56-CDC51C9603FC}</Property>
				</Item>
				<Item Name="TriggerIn" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="ArbitrationForOutputEnable">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputEnable">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXI/PXI_Trig0</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{9F2CFCFF-DE82-427A-803C-3647D5444383}</Property>
				</Item>
			</Item>
			<Item Name="PXIe" Type="Folder">
				<Item Name="PXIe_DStarB" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXIe/PXIe_DStarB</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{3A96F3E5-B6B5-412B-B1C8-0FDB7CF45049}</Property>
				</Item>
				<Item Name="PXIe_DStarC" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXIe/PXIe_DStarC</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{AC2C9003-5340-43DC-8301-B31AA0709F74}</Property>
				</Item>
				<Item Name="PXIe_Sync100" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/PXIe/PXIe_Sync100</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{5772F6AE-B96B-4D58-B490-B196E43795EB}</Property>
				</Item>
			</Item>
			<Item Name="IO Module" Type="FPGA Component Level IP">
				<Property Name="FPGA.PersistentID" Type="Str">{F94814D9-2569-44A7-95EE-6F9AD43FF8DA}</Property>
				<Property Name="NI.FPGA.79XXR.NormalizeCLIPPath" Type="Str">true</Property>
				<Property Name="NI.LV.CLIP.ClockConnections" Type="Xml">
<CLIPConnections>
   <CLIPSignal name="Clock200">
      <ClockFromCLIP>false</ClockFromCLIP>
      <Direction>ToCLIP</Direction>
      <HDLName>Clock200</HDLName>
      <LinkToFPGAClock>200 MHz Clock</LinkToFPGAClock>
      <MaxFreq>205000000.0000</MaxFreq>
      <MinFreq>195000000.0000</MinFreq>
      <UseTopClock>false</UseTopClock>
   </CLIPSignal>
   <CLIPSignal name="Clock40">
      <ClockFromCLIP>false</ClockFromCLIP>
      <Direction>ToCLIP</Direction>
      <HDLName>Clock40</HDLName>
      <LinkToFPGAClock>40 MHz Onboard Clock</LinkToFPGAClock>
      <MaxFreq>45000000.00000</MaxFreq>
      <MinFreq>35000000.00000</MinFreq>
      <UseTopClock>false</UseTopClock>
   </CLIPSignal>
</CLIPConnections></Property>
				<Property Name="NI.LV.CLIP.DeclarationCategory" Type="Str">FlexRIO-IOModule</Property>
				<Property Name="NI.LV.CLIP.DeclarationName" Type="Str">NI 5751</Property>
				<Property Name="NI.LV.CLIP.ExtendedConfigString" Type="Str">IOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA</Property>
				<Property Name="NI.LV.CLIP.SocketedCLIP" Type="Bool">true</Property>
				<Property Name="NI.LV.CLIP.SocketSelection" Type="Str">FlexRIO-IOModule</Property>
				<Property Name="NI.LV.CLIP.SocketSpecificCompileSignature" Type="Str">67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA</Property>
				<Property Name="NI.LV.CLIP.Version" Type="UInt">4</Property>
				<Property Name="NI.LV.FPGA.CompileConfigString" Type="Str">67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
</Property>
				<Property Name="NI.LV.FPGA.Valid" Type="Bool">true</Property>
				<Property Name="NI.SortType" Type="Int">3</Property>
				<Item Name="AI A0" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI A0</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{619FE4DA-6660-465D-A1E9-3FE7D2522486}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI A1" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI A1</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{9F436645-C048-4925-BD00-0E3AD9A6C808}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI A2" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI A2</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{E7A76451-50BC-4789-A331-3447EA58AF7F}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI A3" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI A3</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{7BCE8608-B87E-4C6D-93D5-000D317E7AA8}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI A4" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI A4</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{39950353-3128-42D1-9CDC-61997DA35418}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI A5" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI A5</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{4106B0CB-1151-4A22-85E9-0B95AA3AC17C}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI A6" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI A6</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{70D358EB-0B4B-40E0-BF7C-C10B6CFB5C52}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI A7" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI A7</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{B468824C-28E9-4D74-8E62-8879CA75640B}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI B8" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI B8</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{251B0674-6FD8-4BBE-A7C7-0938E8B45849}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI B9" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI B9</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{AF2B5481-152F-44AC-8B5D-992A5CA04752}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI B10" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI B10</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{8369A44D-B440-444E-AB26-86B545110EF2}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI B11" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI B11</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{67C0686C-7710-46FB-ACE5-4EF8A4C3D442}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI B12" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI B12</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{7CC27B7E-5B7F-49D2-99F4-AC26D2620C05}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI B13" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI B13</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{35EC8E5E-F58D-4D69-97C9-1FAB856A706A}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI B14" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI B14</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{AB50E9C5-6490-47A3-B625-EC6D1143DD07}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="AI B15" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/AI B15</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{7A15B218-6947-42A8-97AA-F1137906021B}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="ADC Error A" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/ADC Error A</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{31AB561C-FFEA-4C03-AE81-45435E23A54A}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="ADC Error B" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/ADC Error B</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{459F9D74-5655-43B8-AC25-C2DA57E54F40}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="Pll Unlocked" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/Pll Unlocked</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{B77D96A7-3EDB-4BB7-B5D8-44BB5D533568}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="Force Initialization" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/Force Initialization</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{C1DD7BF0-6A1E-4C73-A871-1428CB5F8FFA}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="Initialization Done" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/Initialization Done</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{4F28F572-BB2E-44A8-ADBC-EE7C83B22EF6}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="SPI Idle" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/SPI Idle</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{31D220AC-2743-4E8A-A2F1-3CA7704D7673}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="SPI Device Select" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/SPI Device Select</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{8DC9D4EC-7AA6-447B-A3CD-5CDA0E44BE12}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="SPI Address" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/SPI Address</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{083AA03D-0D34-47EF-9BC1-8C944B14D2CC}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="SPI Write Data" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/SPI Write Data</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{74A8DC07-8A69-4F6E-AD03-98F9BF18AC77}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="SPI Write" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/SPI Write</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{5814C834-0EF7-481A-A5AE-43F1C2319C46}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="Sample Clock Select" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>0</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/Sample Clock Select</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{47082E8D-E476-45C6-950A-23BCCC63BCC0}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="DI 0" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/DI 0</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{7F40C34B-007E-4544-934E-1C3B97EE55EB}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="DI 1" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/DI 1</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{549D72D5-A63D-42BA-A51C-CCFB6516D40C}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="DI 2" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/DI 2</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{D476AA2C-1E6A-4981-9C40-37061CAD7CE8}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="DI 3" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/DI 3</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{B8D18856-BD94-451A-BA55-6CBD6646F171}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="DI 4" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/DI 4</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{09BC2919-DADF-47D6-8A54-2EEB64346FB8}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="DI 5" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/DI 5</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{81049F1A-582A-465B-ACDE-DB4332124514}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="DI 6" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/DI 6</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{58EBBC03-D469-47F3-AE9C-8A06EEA78C28}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="DI 7" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="NumberOfSyncRegistersForReadInProject">
   <Value>Auto</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/DI 7</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{5BF07CC5-1944-4440-B50F-B3991EDBB20B}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="D0 0" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/D0 0</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{83C7C6DB-B0EF-4196-8464-2C58C524A6A4}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="D0 1" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/D0 1</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{D4FCC07B-E776-4BDA-ADB8-042D662B05FB}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="D0 2" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/D0 2</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{A542AAE4-16D6-4F09-AFD0-4D67EFCD22B6}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="D0 3" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/D0 3</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{E29B5FF1-BB25-4515-81C9-09267F585B77}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="D0 4" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/D0 4</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{ED2A02E6-8B51-4F86-87DC-A026793EB745}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="D0 5" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/D0 5</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{EC07F106-2868-4BBF-A650-59DBEB62B855}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="D0 6" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/D0 6</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{B8B3D13C-F54E-4349-A5B1-A78C4151576D}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="D0 7" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/D0 7</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{059E4B14-88FE-4D83-A68D-D044D78DABCF}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
				<Item Name="Digital Output Enable" Type="Elemental IO">
					<Property Name="eioAttrBag" Type="Xml"><AttributeSet name="">
   <Attribute name="ArbitrationForOutputData">
   <Value>NeverArbitrate</Value>
   </Attribute>
   <Attribute name="NumberOfSyncRegistersForOutputData">
   <Value>1</Value>
   </Attribute>
   <Attribute name="resource">
   <Value>/IO Module/SignalList/Digital Output Enable</Value>
   </Attribute>
</AttributeSet>
</Property>
					<Property Name="FPGA.PersistentID" Type="Str">{B8822C26-F841-4056-8CFB-894B5C5BF477}</Property>
					<Property Name="NI.LV.EIO.Physical" Type="Bool">true</Property>
				</Item>
			</Item>
			<Item Name="40 MHz Onboard Clock" Type="FPGA Base Clock">
				<Property Name="FPGA.PersistentID" Type="Str">{346C2B77-65E0-4C38-B075-E94E9A442EFE}</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig" Type="Str">ResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.Accuracy" Type="Dbl">100</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.ClockSignalName" Type="Str">RioClk40</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MaxDutyCycle" Type="Dbl">50</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MaxFrequency" Type="Dbl">40000000</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MinDutyCycle" Type="Dbl">50</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MinFrequency" Type="Dbl">40000000</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.NominalFrequency" Type="Dbl">40000000</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.PeakPeriodJitter" Type="Dbl">250</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.ResourceName" Type="Str">40 MHz Onboard Clock</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.SupportAndRequireRuntimeEnableDisable" Type="Bool">false</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.TopSignalConnect" Type="Str">RioClk40</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.VariableFrequency" Type="Bool">false</Property>
				<Property Name="NI.LV.FPGA.Valid" Type="Bool">true</Property>
				<Property Name="NI.LV.FPGA.Version" Type="Int">5</Property>
				<Item Name="10MHz" Type="FPGA Derived Clock">
					<Property Name="FPGA.PersistentID" Type="Str">{B28EC9CC-C123-4FC3-9DAC-E68CE7C4FE0E}</Property>
					<Property Name="NI.LV.FPGA.DerivedConfig" Type="Str">Multiplier=1.000000;Divisor=4.000000</Property>
					<Property Name="NI.LV.FPGA.DerivedConfig.Divisor" Type="Dbl">4</Property>
					<Property Name="NI.LV.FPGA.DerivedConfig.FromExternalClock" Type="Bool">false</Property>
					<Property Name="NI.LV.FPGA.DerivedConfig.Multiplier" Type="Dbl">1</Property>
					<Property Name="NI.LV.FPGA.DerivedConfig.ParentFrequencyInHertz" Type="Dbl">40000000</Property>
					<Property Name="NI.LV.FPGA.Valid" Type="Bool">true</Property>
					<Property Name="NI.LV.FPGA.Version" Type="Int">5</Property>
				</Item>
			</Item>
			<Item Name="200 MHz Clock" Type="FPGA Base Clock">
				<Property Name="FPGA.PersistentID" Type="Str">{C6F16304-3899-48C2-88BF-8338B22C7D08}</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig" Type="Str">ResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.Accuracy" Type="Dbl">100</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.ClockSignalName" Type="Str">DramClk200</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MaxDutyCycle" Type="Dbl">50</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MaxFrequency" Type="Dbl">200000000</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MinDutyCycle" Type="Dbl">50</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MinFrequency" Type="Dbl">200000000</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.NominalFrequency" Type="Dbl">200000000</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.PeakPeriodJitter" Type="Dbl">250</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.ResourceName" Type="Str">200 MHz Clock</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.SupportAndRequireRuntimeEnableDisable" Type="Bool">false</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.TopSignalConnect" Type="Str">DramClk200</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.VariableFrequency" Type="Bool">false</Property>
				<Property Name="NI.LV.FPGA.Valid" Type="Bool">true</Property>
				<Property Name="NI.LV.FPGA.Version" Type="Int">5</Property>
			</Item>
			<Item Name="IO Module Clock 0" Type="FPGA Base Clock">
				<Property Name="FPGA.PersistentID" Type="Str">{C9B9582B-FD64-446D-A112-B7ADBD7ECB62}</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig" Type="Str">ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.Accuracy" Type="Dbl">100</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.ClockSignalName" Type="Str">LvFpgaIoModClipClock0</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MaxDutyCycle" Type="Dbl">55</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MaxFrequency" Type="Dbl">500000000</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MinDutyCycle" Type="Dbl">45</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.MinFrequency" Type="Dbl">50000</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.NominalFrequency" Type="Dbl">50000000</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.PeakPeriodJitter" Type="Dbl">250</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.ResourceName" Type="Str">IO Module Clock 0</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.SupportAndRequireRuntimeEnableDisable" Type="Bool">false</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.TopSignalConnect" Type="Str">LvFpgaIoModClipClock0</Property>
				<Property Name="NI.LV.FPGA.BaseTSConfig.VariableFrequency" Type="Bool">false</Property>
				<Property Name="NI.LV.FPGA.Valid" Type="Bool">true</Property>
				<Property Name="NI.LV.FPGA.Version" Type="Int">5</Property>
			</Item>
			<Item Name="DRAM Bank 0" Type="FPGA Component Level IP">
				<Property Name="NI.LV.CLIP.DeclarationCategory" Type="Str"></Property>
				<Property Name="NI.LV.CLIP.SocketedCLIP" Type="Bool">true</Property>
				<Property Name="NI.LV.CLIP.SocketSelection" Type="Str">FlexRIO-DRAMTypeB-Bank0</Property>
				<Property Name="NI.LV.CLIP.SocketSpecificCompileSignature" Type="Str"></Property>
				<Property Name="NI.LV.CLIP.Version" Type="UInt">4</Property>
				<Property Name="NI.LV.FPGA.Valid" Type="Bool">true</Property>
				<Property Name="NI.SortType" Type="Int">3</Property>
			</Item>
			<Item Name="DRAM Bank 1" Type="FPGA Component Level IP">
				<Property Name="NI.LV.CLIP.DeclarationCategory" Type="Str"></Property>
				<Property Name="NI.LV.CLIP.SocketedCLIP" Type="Bool">true</Property>
				<Property Name="NI.LV.CLIP.SocketSelection" Type="Str">FlexRIO-DRAMTypeB-Bank1</Property>
				<Property Name="NI.LV.CLIP.SocketSpecificCompileSignature" Type="Str"></Property>
				<Property Name="NI.LV.CLIP.Version" Type="UInt">4</Property>
				<Property Name="NI.LV.FPGA.Valid" Type="Bool">true</Property>
				<Property Name="NI.SortType" Type="Int">3</Property>
			</Item>
			<Item Name="InterloopFIFO" Type="FPGA FIFO">
				<Property Name="Actual Number of Elements" Type="UInt">1029</Property>
				<Property Name="Arbitration for Read" Type="UInt">1</Property>
				<Property Name="Arbitration for Write" Type="UInt">1</Property>
				<Property Name="Control Logic" Type="UInt">0</Property>
				<Property Name="Data Type" Type="UInt">8</Property>
				<Property Name="Disable on Overflow/Underflow" Type="Bool">false</Property>
				<Property Name="fifo.configuration" Type="Str">"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"</Property>
				<Property Name="fifo.configured" Type="Bool">true</Property>
				<Property Name="fifo.projectItemValid" Type="Bool">true</Property>
				<Property Name="fifo.valid" Type="Bool">true</Property>
				<Property Name="fifo.version" Type="Int">12</Property>
				<Property Name="FPGA.PersistentID" Type="Str">{CFFC33B9-2530-4A46-AF8F-CF91E059B6E3}</Property>
				<Property Name="Local" Type="Bool">false</Property>
				<Property Name="Memory Type" Type="UInt">2</Property>
				<Property Name="Number Of Elements Per Read" Type="UInt">1</Property>
				<Property Name="Number Of Elements Per Write" Type="UInt">1</Property>
				<Property Name="Requested Number of Elements" Type="UInt">1029</Property>
				<Property Name="Type" Type="UInt">0</Property>
				<Property Name="Type Descriptor" Type="Str">100080000000000100094008000355363400010000000000000000000000000000</Property>
			</Item>
			<Item Name="ToHostCh1FIFO" Type="FPGA FIFO">
				<Property Name="Actual Number of Elements" Type="UInt">8191</Property>
				<Property Name="Arbitration for Read" Type="UInt">1</Property>
				<Property Name="Arbitration for Write" Type="UInt">1</Property>
				<Property Name="Control Logic" Type="UInt">0</Property>
				<Property Name="Data Type" Type="UInt">7</Property>
				<Property Name="Disable on Overflow/Underflow" Type="Bool">false</Property>
				<Property Name="fifo.configuration" Type="Str">"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"</Property>
				<Property Name="fifo.configured" Type="Bool">true</Property>
				<Property Name="fifo.projectItemValid" Type="Bool">true</Property>
				<Property Name="fifo.valid" Type="Bool">true</Property>
				<Property Name="fifo.version" Type="Int">12</Property>
				<Property Name="FPGA.PersistentID" Type="Str">{6CE8D09A-52D3-4D89-89B8-14A0AFFCC65C}</Property>
				<Property Name="Local" Type="Bool">false</Property>
				<Property Name="Memory Type" Type="UInt">2</Property>
				<Property Name="Number Of Elements Per Read" Type="UInt">1</Property>
				<Property Name="Number Of Elements Per Write" Type="UInt">1</Property>
				<Property Name="Requested Number of Elements" Type="UInt">8191</Property>
				<Property Name="Type" Type="UInt">2</Property>
				<Property Name="Type Descriptor" Type="Str">1000800000000001000940070003553332000100000000000000000000</Property>
			</Item>
			<Item Name="ToHostCh2FIFO" Type="FPGA FIFO">
				<Property Name="Actual Number of Elements" Type="UInt">8191</Property>
				<Property Name="Arbitration for Read" Type="UInt">1</Property>
				<Property Name="Arbitration for Write" Type="UInt">1</Property>
				<Property Name="Control Logic" Type="UInt">0</Property>
				<Property Name="Data Type" Type="UInt">7</Property>
				<Property Name="Disable on Overflow/Underflow" Type="Bool">false</Property>
				<Property Name="fifo.configuration" Type="Str">"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"</Property>
				<Property Name="fifo.configured" Type="Bool">true</Property>
				<Property Name="fifo.projectItemValid" Type="Bool">true</Property>
				<Property Name="fifo.valid" Type="Bool">true</Property>
				<Property Name="fifo.version" Type="Int">12</Property>
				<Property Name="FPGA.PersistentID" Type="Str">{38A206DF-3763-4660-AE58-3A1E9221B8F1}</Property>
				<Property Name="Local" Type="Bool">false</Property>
				<Property Name="Memory Type" Type="UInt">2</Property>
				<Property Name="Number Of Elements Per Read" Type="UInt">1</Property>
				<Property Name="Number Of Elements Per Write" Type="UInt">1</Property>
				<Property Name="Requested Number of Elements" Type="UInt">8191</Property>
				<Property Name="Type" Type="UInt">2</Property>
				<Property Name="Type Descriptor" Type="Str">1000800000000001000940070003553332000100000000000000000000</Property>
			</Item>
			<Item Name="ResonanceScanner.vi" Type="VI" URL="../FPGA VIs/ResonanceScanner.vi">
				<Property Name="BuildSpec" Type="Str">{1894403D-6239-4912-82B8-445B967DB106}</Property>
				<Property Name="configString.guid" Type="Str">{059E4B14-88FE-4D83-A68D-D044D78DABCF}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=bool{07169EDD-07AE-44E5-B31A-8F897BFC26B3}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=bool{083AA03D-0D34-47EF-9BC1-8C944B14D2CC}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16{09BC2919-DADF-47D6-8A54-2EEB64346FB8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=bool{251B0674-6FD8-4BBE-A7C7-0938E8B45849}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16{2D78DFA7-362E-4C80-A916-D97D3A069FB8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=bool{31AB561C-FFEA-4C03-AE81-45435E23A54A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=bool{31D220AC-2743-4E8A-A2F1-3CA7704D7673}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=bool{346C2B77-65E0-4C38-B075-E94E9A442EFE}ResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{35EC8E5E-F58D-4D69-97C9-1FAB856A706A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16{38A206DF-3763-4660-AE58-3A1E9221B8F1}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{39950353-3128-42D1-9CDC-61997DA35418}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16{39A12BEA-5368-4BB4-98E8-5559E07C9E74}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32{3A96F3E5-B6B5-412B-B1C8-0FDB7CF45049}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=bool{4106B0CB-1151-4A22-85E9-0B95AA3AC17C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16{459F9D74-5655-43B8-AC25-C2DA57E54F40}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=bool{47082E8D-E476-45C6-950A-23BCCC63BCC0}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8{4F28F572-BB2E-44A8-ADBC-EE7C83B22EF6}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=bool{4F63B79B-75FD-49CE-91E9-FFE9D85A281D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=bool{549D72D5-A63D-42BA-A51C-CCFB6516D40C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=bool{5772F6AE-B96B-4D58-B490-B196E43795EB}NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=bool{5814C834-0EF7-481A-A5AE-43F1C2319C46}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=bool{58EBBC03-D469-47F3-AE9C-8A06EEA78C28}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=bool{5BF07CC5-1944-4440-B50F-B3991EDBB20B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=bool{619FE4DA-6660-465D-A1E9-3FE7D2522486}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16{67C0686C-7710-46FB-ACE5-4EF8A4C3D442}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16{6CE8D09A-52D3-4D89-89B8-14A0AFFCC65C}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{70D358EB-0B4B-40E0-BF7C-C10B6CFB5C52}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16{74A8DC07-8A69-4F6E-AD03-98F9BF18AC77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16{76A096C4-C54C-4822-9ACA-439BD134E65E}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=bool{7A15B218-6947-42A8-97AA-F1137906021B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16{7BCE8608-B87E-4C6D-93D5-000D317E7AA8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16{7CC27B7E-5B7F-49D2-99F4-AC26D2620C05}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16{7EF48D8A-1162-479C-9A48-04F0D5D6AFBD}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32{7F40C34B-007E-4544-934E-1C3B97EE55EB}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=bool{81049F1A-582A-465B-ACDE-DB4332124514}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=bool{8369A44D-B440-444E-AB26-86B545110EF2}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16{83C7C6DB-B0EF-4196-8464-2C58C524A6A4}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=bool{8DC9D4EC-7AA6-447B-A3CD-5CDA0E44BE12}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8{963909BA-5DCF-4D97-857A-450B8BACDC4A}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=bool{9F2CFCFF-DE82-427A-803C-3647D5444383}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool{9F436645-C048-4925-BD00-0E3AD9A6C808}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16{A542AAE4-16D6-4F09-AFD0-4D67EFCD22B6}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=bool{AB50E9C5-6490-47A3-B625-EC6D1143DD07}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16{AC2C9003-5340-43DC-8301-B31AA0709F74}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=bool{AF2B5481-152F-44AC-8B5D-992A5CA04752}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16{B28EC9CC-C123-4FC3-9DAC-E68CE7C4FE0E}Multiplier=1.000000;Divisor=4.000000{B4233A0B-431B-4E58-A4DA-40987CFB4141}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=bool{B468824C-28E9-4D74-8E62-8879CA75640B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16{B64EA97B-7C27-48B4-AD56-CDC51C9603FC}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=bool{B77D96A7-3EDB-4BB7-B5D8-44BB5D533568}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=bool{B8822C26-F841-4056-8CFB-894B5C5BF477}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=bool{B8B3D13C-F54E-4349-A5B1-A78C4151576D}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=bool{B8D18856-BD94-451A-BA55-6CBD6646F171}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=bool{C1DD7BF0-6A1E-4C73-A871-1428CB5F8FFA}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=bool{C6F16304-3899-48C2-88BF-8338B22C7D08}ResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{C7027B38-BA2C-4A7A-867E-31F526CEC5D8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=bool{C9B9582B-FD64-446D-A112-B7ADBD7ECB62}ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{CFFC33B9-2530-4A46-AF8F-CF91E059B6E3}"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"{D4007612-93E7-4F24-8BF7-055EDA438857}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=bool{D476AA2C-1E6A-4981-9C40-37061CAD7CE8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=bool{D4FCC07B-E776-4BDA-ADB8-042D662B05FB}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=bool{D57DFE40-3C0D-424A-96DB-8F4B76E5181E}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=bool{D75C180A-7216-45AD-8B38-C6702DF1BE36}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16{E29B5FF1-BB25-4515-81C9-09267F585B77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=bool{E580D444-134F-4148-B7A4-665A022C06DE}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=bool{E7A76451-50BC-4789-A331-3447EA58AF7F}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16{EBD4042E-911B-4524-8CA2-FC053981E6C6}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=bool{EC07F106-2868-4BBF-A650-59DBEB62B855}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=bool{ED2A02E6-8B51-4F86-87DC-A026793EB745}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=bool{EE7BDDBE-7382-4C23-A4F5-71DBAFDE6DE4}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=bool{F830D38C-B78A-4DEA-BFB7-1F41DB00204F}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=bool{F94814D9-2569-44A7-95EE-6F9AD43FF8DA}67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
{FC0A9E4C-5E7A-4CF8-8384-A4346539B90D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGA</Property>
				<Property Name="configString.name" Type="Str">10MHzMultiplier=1.000000;Divisor=4.000000200 MHz ClockResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E40 MHz Onboard ClockResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EADC Error ANumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=boolADC Error BNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=boolAI A0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16AI A1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16AI A2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16AI A3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16AI A4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16AI A5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16AI A6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16AI A7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16AI B10NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16AI B11NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16AI B12NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16AI B13NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16AI B14NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16AI B15NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16AI B8NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16AI B9NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16Clock100 PLL UnlockedNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=boolD0 0ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=boolD0 1ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=boolD0 2ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=boolD0 3ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=boolD0 4ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=boolD0 5ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=boolD0 6ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=boolD0 7ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=boolDevice TemperatureNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16DI 0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=boolDI 1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=boolDI 2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=boolDI 3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=boolDI 4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=boolDI 5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=boolDI 6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=boolDI 7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=boolDigital Output EnableArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=boolEEPROM Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=boolExpected IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32Force InitializationArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=boolInitialization DoneNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=boolInserted IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32InterloopFIFO"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"IO Module Clock 0ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EIO Module IO EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=boolIO Module Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=boolIO Module Power GoodNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolIO Module PresentNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=boolIO Module67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
LineClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=boolPixelClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=boolPll UnlockedNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=boolPXI_Clk10NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=boolPXI_StarNumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=boolPXI_Trig3ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig4ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig5ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig6ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig7ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=boolPXIe_DStarBNumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=boolPXIe_DStarCArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=boolPXIe_Sync100NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGASample Clock SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8SPI AddressArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16SPI Device SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8SPI IdleNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=boolSPI Write DataArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16SPI WriteArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=boolToHostCh1FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"ToHostCh2FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"TriggerInArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool</Property>
				<Property Name="NI.LV.FPGA.InterfaceBitfile" Type="Str">C:\Scope\fpga_labview\FPGA Bitfiles\ResonanceScanner_PXIe-7962R.lvbitx</Property>
			</Item>
			<Item Name="Host_Test.vi" Type="VI" URL="../Host_Test.vi">
				<Property Name="configString.guid" Type="Str">{059E4B14-88FE-4D83-A68D-D044D78DABCF}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=bool{07169EDD-07AE-44E5-B31A-8F897BFC26B3}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=bool{083AA03D-0D34-47EF-9BC1-8C944B14D2CC}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16{09BC2919-DADF-47D6-8A54-2EEB64346FB8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=bool{251B0674-6FD8-4BBE-A7C7-0938E8B45849}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16{2D78DFA7-362E-4C80-A916-D97D3A069FB8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=bool{31AB561C-FFEA-4C03-AE81-45435E23A54A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=bool{31D220AC-2743-4E8A-A2F1-3CA7704D7673}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=bool{346C2B77-65E0-4C38-B075-E94E9A442EFE}ResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{35EC8E5E-F58D-4D69-97C9-1FAB856A706A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16{38A206DF-3763-4660-AE58-3A1E9221B8F1}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{39950353-3128-42D1-9CDC-61997DA35418}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16{39A12BEA-5368-4BB4-98E8-5559E07C9E74}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32{3A96F3E5-B6B5-412B-B1C8-0FDB7CF45049}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=bool{4106B0CB-1151-4A22-85E9-0B95AA3AC17C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16{459F9D74-5655-43B8-AC25-C2DA57E54F40}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=bool{47082E8D-E476-45C6-950A-23BCCC63BCC0}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8{4F28F572-BB2E-44A8-ADBC-EE7C83B22EF6}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=bool{4F63B79B-75FD-49CE-91E9-FFE9D85A281D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=bool{549D72D5-A63D-42BA-A51C-CCFB6516D40C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=bool{5772F6AE-B96B-4D58-B490-B196E43795EB}NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=bool{5814C834-0EF7-481A-A5AE-43F1C2319C46}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=bool{58EBBC03-D469-47F3-AE9C-8A06EEA78C28}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=bool{5BF07CC5-1944-4440-B50F-B3991EDBB20B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=bool{619FE4DA-6660-465D-A1E9-3FE7D2522486}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16{67C0686C-7710-46FB-ACE5-4EF8A4C3D442}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16{6CE8D09A-52D3-4D89-89B8-14A0AFFCC65C}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{70D358EB-0B4B-40E0-BF7C-C10B6CFB5C52}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16{74A8DC07-8A69-4F6E-AD03-98F9BF18AC77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16{76A096C4-C54C-4822-9ACA-439BD134E65E}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=bool{7A15B218-6947-42A8-97AA-F1137906021B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16{7BCE8608-B87E-4C6D-93D5-000D317E7AA8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16{7CC27B7E-5B7F-49D2-99F4-AC26D2620C05}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16{7EF48D8A-1162-479C-9A48-04F0D5D6AFBD}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32{7F40C34B-007E-4544-934E-1C3B97EE55EB}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=bool{81049F1A-582A-465B-ACDE-DB4332124514}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=bool{8369A44D-B440-444E-AB26-86B545110EF2}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16{83C7C6DB-B0EF-4196-8464-2C58C524A6A4}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=bool{8DC9D4EC-7AA6-447B-A3CD-5CDA0E44BE12}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8{963909BA-5DCF-4D97-857A-450B8BACDC4A}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=bool{9F2CFCFF-DE82-427A-803C-3647D5444383}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool{9F436645-C048-4925-BD00-0E3AD9A6C808}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16{A542AAE4-16D6-4F09-AFD0-4D67EFCD22B6}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=bool{AB50E9C5-6490-47A3-B625-EC6D1143DD07}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16{AC2C9003-5340-43DC-8301-B31AA0709F74}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=bool{AF2B5481-152F-44AC-8B5D-992A5CA04752}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16{B28EC9CC-C123-4FC3-9DAC-E68CE7C4FE0E}Multiplier=1.000000;Divisor=4.000000{B4233A0B-431B-4E58-A4DA-40987CFB4141}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=bool{B468824C-28E9-4D74-8E62-8879CA75640B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16{B64EA97B-7C27-48B4-AD56-CDC51C9603FC}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=bool{B77D96A7-3EDB-4BB7-B5D8-44BB5D533568}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=bool{B8822C26-F841-4056-8CFB-894B5C5BF477}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=bool{B8B3D13C-F54E-4349-A5B1-A78C4151576D}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=bool{B8D18856-BD94-451A-BA55-6CBD6646F171}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=bool{C1DD7BF0-6A1E-4C73-A871-1428CB5F8FFA}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=bool{C6F16304-3899-48C2-88BF-8338B22C7D08}ResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{C7027B38-BA2C-4A7A-867E-31F526CEC5D8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=bool{C9B9582B-FD64-446D-A112-B7ADBD7ECB62}ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{CFFC33B9-2530-4A46-AF8F-CF91E059B6E3}"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"{D4007612-93E7-4F24-8BF7-055EDA438857}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=bool{D476AA2C-1E6A-4981-9C40-37061CAD7CE8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=bool{D4FCC07B-E776-4BDA-ADB8-042D662B05FB}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=bool{D57DFE40-3C0D-424A-96DB-8F4B76E5181E}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=bool{D75C180A-7216-45AD-8B38-C6702DF1BE36}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16{E29B5FF1-BB25-4515-81C9-09267F585B77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=bool{E580D444-134F-4148-B7A4-665A022C06DE}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=bool{E7A76451-50BC-4789-A331-3447EA58AF7F}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16{EBD4042E-911B-4524-8CA2-FC053981E6C6}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=bool{EC07F106-2868-4BBF-A650-59DBEB62B855}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=bool{ED2A02E6-8B51-4F86-87DC-A026793EB745}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=bool{EE7BDDBE-7382-4C23-A4F5-71DBAFDE6DE4}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=bool{F830D38C-B78A-4DEA-BFB7-1F41DB00204F}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=bool{F94814D9-2569-44A7-95EE-6F9AD43FF8DA}67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
{FC0A9E4C-5E7A-4CF8-8384-A4346539B90D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGA</Property>
				<Property Name="configString.name" Type="Str">10MHzMultiplier=1.000000;Divisor=4.000000200 MHz ClockResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E40 MHz Onboard ClockResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EADC Error ANumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=boolADC Error BNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=boolAI A0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16AI A1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16AI A2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16AI A3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16AI A4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16AI A5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16AI A6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16AI A7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16AI B10NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16AI B11NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16AI B12NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16AI B13NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16AI B14NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16AI B15NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16AI B8NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16AI B9NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16Clock100 PLL UnlockedNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=boolD0 0ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=boolD0 1ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=boolD0 2ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=boolD0 3ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=boolD0 4ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=boolD0 5ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=boolD0 6ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=boolD0 7ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=boolDevice TemperatureNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16DI 0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=boolDI 1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=boolDI 2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=boolDI 3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=boolDI 4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=boolDI 5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=boolDI 6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=boolDI 7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=boolDigital Output EnableArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=boolEEPROM Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=boolExpected IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32Force InitializationArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=boolInitialization DoneNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=boolInserted IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32InterloopFIFO"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"IO Module Clock 0ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EIO Module IO EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=boolIO Module Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=boolIO Module Power GoodNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolIO Module PresentNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=boolIO Module67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
LineClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=boolPixelClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=boolPll UnlockedNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=boolPXI_Clk10NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=boolPXI_StarNumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=boolPXI_Trig3ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig4ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig5ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig6ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig7ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=boolPXIe_DStarBNumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=boolPXIe_DStarCArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=boolPXIe_Sync100NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGASample Clock SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8SPI AddressArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16SPI Device SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8SPI IdleNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=boolSPI Write DataArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16SPI WriteArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=boolToHostCh1FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"ToHostCh2FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"TriggerInArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool</Property>
			</Item>
			<Item Name="FIFOCluster.ctl" Type="VI" URL="../FPGA VIs/FIFOCluster.ctl">
				<Property Name="configString.guid" Type="Str">{059E4B14-88FE-4D83-A68D-D044D78DABCF}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=bool{07169EDD-07AE-44E5-B31A-8F897BFC26B3}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=bool{083AA03D-0D34-47EF-9BC1-8C944B14D2CC}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16{09BC2919-DADF-47D6-8A54-2EEB64346FB8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=bool{251B0674-6FD8-4BBE-A7C7-0938E8B45849}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16{2D78DFA7-362E-4C80-A916-D97D3A069FB8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=bool{31AB561C-FFEA-4C03-AE81-45435E23A54A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=bool{31D220AC-2743-4E8A-A2F1-3CA7704D7673}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=bool{346C2B77-65E0-4C38-B075-E94E9A442EFE}ResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{35EC8E5E-F58D-4D69-97C9-1FAB856A706A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16{38A206DF-3763-4660-AE58-3A1E9221B8F1}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{39950353-3128-42D1-9CDC-61997DA35418}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16{39A12BEA-5368-4BB4-98E8-5559E07C9E74}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32{3A96F3E5-B6B5-412B-B1C8-0FDB7CF45049}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=bool{4106B0CB-1151-4A22-85E9-0B95AA3AC17C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16{459F9D74-5655-43B8-AC25-C2DA57E54F40}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=bool{47082E8D-E476-45C6-950A-23BCCC63BCC0}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8{4F28F572-BB2E-44A8-ADBC-EE7C83B22EF6}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=bool{4F63B79B-75FD-49CE-91E9-FFE9D85A281D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=bool{549D72D5-A63D-42BA-A51C-CCFB6516D40C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=bool{5772F6AE-B96B-4D58-B490-B196E43795EB}NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=bool{5814C834-0EF7-481A-A5AE-43F1C2319C46}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=bool{58EBBC03-D469-47F3-AE9C-8A06EEA78C28}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=bool{5BF07CC5-1944-4440-B50F-B3991EDBB20B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=bool{619FE4DA-6660-465D-A1E9-3FE7D2522486}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16{67C0686C-7710-46FB-ACE5-4EF8A4C3D442}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16{6CE8D09A-52D3-4D89-89B8-14A0AFFCC65C}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{70D358EB-0B4B-40E0-BF7C-C10B6CFB5C52}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16{74A8DC07-8A69-4F6E-AD03-98F9BF18AC77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16{76A096C4-C54C-4822-9ACA-439BD134E65E}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=bool{7A15B218-6947-42A8-97AA-F1137906021B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16{7BCE8608-B87E-4C6D-93D5-000D317E7AA8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16{7CC27B7E-5B7F-49D2-99F4-AC26D2620C05}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16{7EF48D8A-1162-479C-9A48-04F0D5D6AFBD}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32{7F40C34B-007E-4544-934E-1C3B97EE55EB}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=bool{81049F1A-582A-465B-ACDE-DB4332124514}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=bool{8369A44D-B440-444E-AB26-86B545110EF2}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16{83C7C6DB-B0EF-4196-8464-2C58C524A6A4}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=bool{8DC9D4EC-7AA6-447B-A3CD-5CDA0E44BE12}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8{963909BA-5DCF-4D97-857A-450B8BACDC4A}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=bool{9F2CFCFF-DE82-427A-803C-3647D5444383}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool{9F436645-C048-4925-BD00-0E3AD9A6C808}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16{A542AAE4-16D6-4F09-AFD0-4D67EFCD22B6}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=bool{AB50E9C5-6490-47A3-B625-EC6D1143DD07}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16{AC2C9003-5340-43DC-8301-B31AA0709F74}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=bool{AF2B5481-152F-44AC-8B5D-992A5CA04752}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16{B28EC9CC-C123-4FC3-9DAC-E68CE7C4FE0E}Multiplier=1.000000;Divisor=4.000000{B4233A0B-431B-4E58-A4DA-40987CFB4141}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=bool{B468824C-28E9-4D74-8E62-8879CA75640B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16{B64EA97B-7C27-48B4-AD56-CDC51C9603FC}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=bool{B77D96A7-3EDB-4BB7-B5D8-44BB5D533568}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=bool{B8822C26-F841-4056-8CFB-894B5C5BF477}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=bool{B8B3D13C-F54E-4349-A5B1-A78C4151576D}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=bool{B8D18856-BD94-451A-BA55-6CBD6646F171}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=bool{C1DD7BF0-6A1E-4C73-A871-1428CB5F8FFA}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=bool{C6F16304-3899-48C2-88BF-8338B22C7D08}ResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{C7027B38-BA2C-4A7A-867E-31F526CEC5D8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=bool{C9B9582B-FD64-446D-A112-B7ADBD7ECB62}ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{CFFC33B9-2530-4A46-AF8F-CF91E059B6E3}"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"{D4007612-93E7-4F24-8BF7-055EDA438857}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=bool{D476AA2C-1E6A-4981-9C40-37061CAD7CE8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=bool{D4FCC07B-E776-4BDA-ADB8-042D662B05FB}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=bool{D57DFE40-3C0D-424A-96DB-8F4B76E5181E}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=bool{D75C180A-7216-45AD-8B38-C6702DF1BE36}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16{E29B5FF1-BB25-4515-81C9-09267F585B77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=bool{E580D444-134F-4148-B7A4-665A022C06DE}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=bool{E7A76451-50BC-4789-A331-3447EA58AF7F}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16{EBD4042E-911B-4524-8CA2-FC053981E6C6}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=bool{EC07F106-2868-4BBF-A650-59DBEB62B855}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=bool{ED2A02E6-8B51-4F86-87DC-A026793EB745}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=bool{EE7BDDBE-7382-4C23-A4F5-71DBAFDE6DE4}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=bool{F830D38C-B78A-4DEA-BFB7-1F41DB00204F}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=bool{F94814D9-2569-44A7-95EE-6F9AD43FF8DA}67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
{FC0A9E4C-5E7A-4CF8-8384-A4346539B90D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGA</Property>
				<Property Name="configString.name" Type="Str">10MHzMultiplier=1.000000;Divisor=4.000000200 MHz ClockResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E40 MHz Onboard ClockResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EADC Error ANumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=boolADC Error BNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=boolAI A0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16AI A1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16AI A2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16AI A3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16AI A4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16AI A5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16AI A6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16AI A7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16AI B10NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16AI B11NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16AI B12NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16AI B13NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16AI B14NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16AI B15NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16AI B8NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16AI B9NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16Clock100 PLL UnlockedNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=boolD0 0ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=boolD0 1ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=boolD0 2ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=boolD0 3ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=boolD0 4ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=boolD0 5ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=boolD0 6ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=boolD0 7ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=boolDevice TemperatureNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16DI 0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=boolDI 1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=boolDI 2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=boolDI 3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=boolDI 4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=boolDI 5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=boolDI 6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=boolDI 7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=boolDigital Output EnableArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=boolEEPROM Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=boolExpected IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32Force InitializationArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=boolInitialization DoneNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=boolInserted IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32InterloopFIFO"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"IO Module Clock 0ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EIO Module IO EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=boolIO Module Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=boolIO Module Power GoodNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolIO Module PresentNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=boolIO Module67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
LineClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=boolPixelClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=boolPll UnlockedNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=boolPXI_Clk10NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=boolPXI_StarNumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=boolPXI_Trig3ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig4ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig5ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig6ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig7ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=boolPXIe_DStarBNumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=boolPXIe_DStarCArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=boolPXIe_Sync100NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGASample Clock SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8SPI AddressArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16SPI Device SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8SPI IdleNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=boolSPI Write DataArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16SPI WriteArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=boolToHostCh1FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"ToHostCh2FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"TriggerInArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool</Property>
			</Item>
			<Item Name="add_resonance_scanner_trigger.vi" Type="VI" URL="../SubVIs/add_resonance_scanner_trigger.vi">
				<Property Name="configString.guid" Type="Str">{059E4B14-88FE-4D83-A68D-D044D78DABCF}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=bool{07169EDD-07AE-44E5-B31A-8F897BFC26B3}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=bool{083AA03D-0D34-47EF-9BC1-8C944B14D2CC}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16{09BC2919-DADF-47D6-8A54-2EEB64346FB8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=bool{251B0674-6FD8-4BBE-A7C7-0938E8B45849}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16{2D78DFA7-362E-4C80-A916-D97D3A069FB8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=bool{31AB561C-FFEA-4C03-AE81-45435E23A54A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=bool{31D220AC-2743-4E8A-A2F1-3CA7704D7673}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=bool{346C2B77-65E0-4C38-B075-E94E9A442EFE}ResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{35EC8E5E-F58D-4D69-97C9-1FAB856A706A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16{38A206DF-3763-4660-AE58-3A1E9221B8F1}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{39950353-3128-42D1-9CDC-61997DA35418}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16{39A12BEA-5368-4BB4-98E8-5559E07C9E74}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32{3A96F3E5-B6B5-412B-B1C8-0FDB7CF45049}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=bool{4106B0CB-1151-4A22-85E9-0B95AA3AC17C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16{459F9D74-5655-43B8-AC25-C2DA57E54F40}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=bool{47082E8D-E476-45C6-950A-23BCCC63BCC0}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8{4F28F572-BB2E-44A8-ADBC-EE7C83B22EF6}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=bool{4F63B79B-75FD-49CE-91E9-FFE9D85A281D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=bool{549D72D5-A63D-42BA-A51C-CCFB6516D40C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=bool{5772F6AE-B96B-4D58-B490-B196E43795EB}NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=bool{5814C834-0EF7-481A-A5AE-43F1C2319C46}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=bool{58EBBC03-D469-47F3-AE9C-8A06EEA78C28}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=bool{5BF07CC5-1944-4440-B50F-B3991EDBB20B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=bool{619FE4DA-6660-465D-A1E9-3FE7D2522486}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16{67C0686C-7710-46FB-ACE5-4EF8A4C3D442}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16{6CE8D09A-52D3-4D89-89B8-14A0AFFCC65C}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{70D358EB-0B4B-40E0-BF7C-C10B6CFB5C52}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16{74A8DC07-8A69-4F6E-AD03-98F9BF18AC77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16{76A096C4-C54C-4822-9ACA-439BD134E65E}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=bool{7A15B218-6947-42A8-97AA-F1137906021B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16{7BCE8608-B87E-4C6D-93D5-000D317E7AA8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16{7CC27B7E-5B7F-49D2-99F4-AC26D2620C05}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16{7EF48D8A-1162-479C-9A48-04F0D5D6AFBD}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32{7F40C34B-007E-4544-934E-1C3B97EE55EB}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=bool{81049F1A-582A-465B-ACDE-DB4332124514}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=bool{8369A44D-B440-444E-AB26-86B545110EF2}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16{83C7C6DB-B0EF-4196-8464-2C58C524A6A4}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=bool{8DC9D4EC-7AA6-447B-A3CD-5CDA0E44BE12}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8{963909BA-5DCF-4D97-857A-450B8BACDC4A}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=bool{9F2CFCFF-DE82-427A-803C-3647D5444383}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool{9F436645-C048-4925-BD00-0E3AD9A6C808}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16{A542AAE4-16D6-4F09-AFD0-4D67EFCD22B6}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=bool{AB50E9C5-6490-47A3-B625-EC6D1143DD07}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16{AC2C9003-5340-43DC-8301-B31AA0709F74}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=bool{AF2B5481-152F-44AC-8B5D-992A5CA04752}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16{B28EC9CC-C123-4FC3-9DAC-E68CE7C4FE0E}Multiplier=1.000000;Divisor=4.000000{B4233A0B-431B-4E58-A4DA-40987CFB4141}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=bool{B468824C-28E9-4D74-8E62-8879CA75640B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16{B64EA97B-7C27-48B4-AD56-CDC51C9603FC}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=bool{B77D96A7-3EDB-4BB7-B5D8-44BB5D533568}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=bool{B8822C26-F841-4056-8CFB-894B5C5BF477}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=bool{B8B3D13C-F54E-4349-A5B1-A78C4151576D}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=bool{B8D18856-BD94-451A-BA55-6CBD6646F171}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=bool{C1DD7BF0-6A1E-4C73-A871-1428CB5F8FFA}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=bool{C6F16304-3899-48C2-88BF-8338B22C7D08}ResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{C7027B38-BA2C-4A7A-867E-31F526CEC5D8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=bool{C9B9582B-FD64-446D-A112-B7ADBD7ECB62}ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{CFFC33B9-2530-4A46-AF8F-CF91E059B6E3}"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"{D4007612-93E7-4F24-8BF7-055EDA438857}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=bool{D476AA2C-1E6A-4981-9C40-37061CAD7CE8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=bool{D4FCC07B-E776-4BDA-ADB8-042D662B05FB}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=bool{D57DFE40-3C0D-424A-96DB-8F4B76E5181E}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=bool{D75C180A-7216-45AD-8B38-C6702DF1BE36}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16{E29B5FF1-BB25-4515-81C9-09267F585B77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=bool{E580D444-134F-4148-B7A4-665A022C06DE}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=bool{E7A76451-50BC-4789-A331-3447EA58AF7F}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16{EBD4042E-911B-4524-8CA2-FC053981E6C6}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=bool{EC07F106-2868-4BBF-A650-59DBEB62B855}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=bool{ED2A02E6-8B51-4F86-87DC-A026793EB745}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=bool{EE7BDDBE-7382-4C23-A4F5-71DBAFDE6DE4}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=bool{F830D38C-B78A-4DEA-BFB7-1F41DB00204F}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=bool{F94814D9-2569-44A7-95EE-6F9AD43FF8DA}67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
{FC0A9E4C-5E7A-4CF8-8384-A4346539B90D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGA</Property>
				<Property Name="configString.name" Type="Str">10MHzMultiplier=1.000000;Divisor=4.000000200 MHz ClockResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E40 MHz Onboard ClockResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EADC Error ANumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=boolADC Error BNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=boolAI A0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16AI A1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16AI A2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16AI A3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16AI A4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16AI A5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16AI A6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16AI A7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16AI B10NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16AI B11NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16AI B12NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16AI B13NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16AI B14NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16AI B15NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16AI B8NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16AI B9NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16Clock100 PLL UnlockedNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=boolD0 0ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=boolD0 1ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=boolD0 2ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=boolD0 3ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=boolD0 4ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=boolD0 5ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=boolD0 6ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=boolD0 7ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=boolDevice TemperatureNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16DI 0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=boolDI 1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=boolDI 2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=boolDI 3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=boolDI 4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=boolDI 5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=boolDI 6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=boolDI 7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=boolDigital Output EnableArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=boolEEPROM Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=boolExpected IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32Force InitializationArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=boolInitialization DoneNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=boolInserted IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32InterloopFIFO"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"IO Module Clock 0ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EIO Module IO EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=boolIO Module Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=boolIO Module Power GoodNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolIO Module PresentNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=boolIO Module67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
LineClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=boolPixelClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=boolPll UnlockedNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=boolPXI_Clk10NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=boolPXI_StarNumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=boolPXI_Trig3ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig4ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig5ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig6ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig7ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=boolPXIe_DStarBNumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=boolPXIe_DStarCArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=boolPXIe_Sync100NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGASample Clock SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8SPI AddressArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16SPI Device SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8SPI IdleNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=boolSPI Write DataArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16SPI WriteArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=boolToHostCh1FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"ToHostCh2FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"TriggerInArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool</Property>
			</Item>
			<Item Name="check_forward_scan.vi" Type="VI" URL="../SubVIs/check_forward_scan.vi">
				<Property Name="configString.guid" Type="Str">{059E4B14-88FE-4D83-A68D-D044D78DABCF}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=bool{07169EDD-07AE-44E5-B31A-8F897BFC26B3}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=bool{083AA03D-0D34-47EF-9BC1-8C944B14D2CC}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16{09BC2919-DADF-47D6-8A54-2EEB64346FB8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=bool{251B0674-6FD8-4BBE-A7C7-0938E8B45849}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16{2D78DFA7-362E-4C80-A916-D97D3A069FB8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=bool{31AB561C-FFEA-4C03-AE81-45435E23A54A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=bool{31D220AC-2743-4E8A-A2F1-3CA7704D7673}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=bool{346C2B77-65E0-4C38-B075-E94E9A442EFE}ResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{35EC8E5E-F58D-4D69-97C9-1FAB856A706A}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16{38A206DF-3763-4660-AE58-3A1E9221B8F1}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{39950353-3128-42D1-9CDC-61997DA35418}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16{39A12BEA-5368-4BB4-98E8-5559E07C9E74}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32{3A96F3E5-B6B5-412B-B1C8-0FDB7CF45049}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=bool{4106B0CB-1151-4A22-85E9-0B95AA3AC17C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16{459F9D74-5655-43B8-AC25-C2DA57E54F40}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=bool{47082E8D-E476-45C6-950A-23BCCC63BCC0}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8{4F28F572-BB2E-44A8-ADBC-EE7C83B22EF6}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=bool{4F63B79B-75FD-49CE-91E9-FFE9D85A281D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=bool{549D72D5-A63D-42BA-A51C-CCFB6516D40C}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=bool{5772F6AE-B96B-4D58-B490-B196E43795EB}NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=bool{5814C834-0EF7-481A-A5AE-43F1C2319C46}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=bool{58EBBC03-D469-47F3-AE9C-8A06EEA78C28}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=bool{5BF07CC5-1944-4440-B50F-B3991EDBB20B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=bool{619FE4DA-6660-465D-A1E9-3FE7D2522486}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16{67C0686C-7710-46FB-ACE5-4EF8A4C3D442}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16{6CE8D09A-52D3-4D89-89B8-14A0AFFCC65C}"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"{70D358EB-0B4B-40E0-BF7C-C10B6CFB5C52}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16{74A8DC07-8A69-4F6E-AD03-98F9BF18AC77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16{76A096C4-C54C-4822-9ACA-439BD134E65E}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=bool{7A15B218-6947-42A8-97AA-F1137906021B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16{7BCE8608-B87E-4C6D-93D5-000D317E7AA8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16{7CC27B7E-5B7F-49D2-99F4-AC26D2620C05}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16{7EF48D8A-1162-479C-9A48-04F0D5D6AFBD}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32{7F40C34B-007E-4544-934E-1C3B97EE55EB}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=bool{81049F1A-582A-465B-ACDE-DB4332124514}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=bool{8369A44D-B440-444E-AB26-86B545110EF2}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16{83C7C6DB-B0EF-4196-8464-2C58C524A6A4}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=bool{8DC9D4EC-7AA6-447B-A3CD-5CDA0E44BE12}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8{963909BA-5DCF-4D97-857A-450B8BACDC4A}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=bool{9F2CFCFF-DE82-427A-803C-3647D5444383}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool{9F436645-C048-4925-BD00-0E3AD9A6C808}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16{A542AAE4-16D6-4F09-AFD0-4D67EFCD22B6}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=bool{AB50E9C5-6490-47A3-B625-EC6D1143DD07}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16{AC2C9003-5340-43DC-8301-B31AA0709F74}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=bool{AF2B5481-152F-44AC-8B5D-992A5CA04752}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16{B28EC9CC-C123-4FC3-9DAC-E68CE7C4FE0E}Multiplier=1.000000;Divisor=4.000000{B4233A0B-431B-4E58-A4DA-40987CFB4141}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=bool{B468824C-28E9-4D74-8E62-8879CA75640B}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16{B64EA97B-7C27-48B4-AD56-CDC51C9603FC}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=bool{B77D96A7-3EDB-4BB7-B5D8-44BB5D533568}NumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=bool{B8822C26-F841-4056-8CFB-894B5C5BF477}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=bool{B8B3D13C-F54E-4349-A5B1-A78C4151576D}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=bool{B8D18856-BD94-451A-BA55-6CBD6646F171}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=bool{C1DD7BF0-6A1E-4C73-A871-1428CB5F8FFA}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=bool{C6F16304-3899-48C2-88BF-8338B22C7D08}ResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{C7027B38-BA2C-4A7A-867E-31F526CEC5D8}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=bool{C9B9582B-FD64-446D-A112-B7ADBD7ECB62}ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E{CFFC33B9-2530-4A46-AF8F-CF91E059B6E3}"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"{D4007612-93E7-4F24-8BF7-055EDA438857}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=bool{D476AA2C-1E6A-4981-9C40-37061CAD7CE8}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=bool{D4FCC07B-E776-4BDA-ADB8-042D662B05FB}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=bool{D57DFE40-3C0D-424A-96DB-8F4B76E5181E}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=bool{D75C180A-7216-45AD-8B38-C6702DF1BE36}NumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16{E29B5FF1-BB25-4515-81C9-09267F585B77}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=bool{E580D444-134F-4148-B7A4-665A022C06DE}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=bool{E7A76451-50BC-4789-A331-3447EA58AF7F}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16{EBD4042E-911B-4524-8CA2-FC053981E6C6}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=bool{EC07F106-2868-4BBF-A650-59DBEB62B855}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=bool{ED2A02E6-8B51-4F86-87DC-A026793EB745}ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=bool{EE7BDDBE-7382-4C23-A4F5-71DBAFDE6DE4}ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=bool{F830D38C-B78A-4DEA-BFB7-1F41DB00204F}NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=bool{F94814D9-2569-44A7-95EE-6F9AD43FF8DA}67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
{FC0A9E4C-5E7A-4CF8-8384-A4346539B90D}NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGA</Property>
				<Property Name="configString.name" Type="Str">10MHzMultiplier=1.000000;Divisor=4.000000200 MHz ClockResourceName=200 MHz Clock;TopSignalConnect=DramClk200;ClockSignalName=DramClk200;MinFreq=200000000.000000;MaxFreq=200000000.000000;VariableFreq=0;NomFreq=200000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427E40 MHz Onboard ClockResourceName=40 MHz Onboard Clock;TopSignalConnect=RioClk40;ClockSignalName=RioClk40;MinFreq=40000000.000000;MaxFreq=40000000.000000;VariableFreq=0;NomFreq=40000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=50.000000;MaxDutyCycle=50.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EADC Error ANumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error A;0;ReadMethodType=boolADC Error BNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/ADC Error B;0;ReadMethodType=boolAI A0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A0;0;ReadMethodType=I16AI A1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A1;0;ReadMethodType=I16AI A2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A2;0;ReadMethodType=I16AI A3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A3;0;ReadMethodType=I16AI A4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A4;0;ReadMethodType=I16AI A5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A5;0;ReadMethodType=I16AI A6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A6;0;ReadMethodType=I16AI A7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI A7;0;ReadMethodType=I16AI B10NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B10;0;ReadMethodType=I16AI B11NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B11;0;ReadMethodType=I16AI B12NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B12;0;ReadMethodType=I16AI B13NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B13;0;ReadMethodType=I16AI B14NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B14;0;ReadMethodType=I16AI B15NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B15;0;ReadMethodType=I16AI B8NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B8;0;ReadMethodType=I16AI B9NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/AI B9;0;ReadMethodType=I16Clock100 PLL UnlockedNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Clock100 PLL Unlocked;0;ReadMethodType=boolD0 0ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 0;0;WriteMethodType=boolD0 1ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 1;0;WriteMethodType=boolD0 2ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 2;0;WriteMethodType=boolD0 3ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 3;0;WriteMethodType=boolD0 4ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 4;0;WriteMethodType=boolD0 5ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 5;0;WriteMethodType=boolD0 6ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 6;0;WriteMethodType=boolD0 7ArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/D0 7;0;WriteMethodType=boolDevice TemperatureNumberOfSyncRegistersForReadInProject=Auto;resource=/Board IO/Device Temperature;0;ReadMethodType=i16DI 0NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 0;0;ReadMethodType=boolDI 1NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 1;0;ReadMethodType=boolDI 2NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 2;0;ReadMethodType=boolDI 3NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 3;0;ReadMethodType=boolDI 4NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 4;0;ReadMethodType=boolDI 5NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 5;0;ReadMethodType=boolDI 6NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 6;0;ReadMethodType=boolDI 7NumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module/SignalList/DI 7;0;ReadMethodType=boolDigital Output EnableArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;resource=/IO Module/SignalList/Digital Output Enable;0;WriteMethodType=boolEEPROM Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/EEPROM Power Enabled;0;ReadMethodType=boolExpected IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Expected IO Module ID;0;ReadMethodType=u32Force InitializationArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Force Initialization;0;WriteMethodType=boolInitialization DoneNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Initialization Done;0;ReadMethodType=boolInserted IO Module IDNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/Inserted IO Module ID;0;ReadMethodType=u32InterloopFIFO"ControlLogic=0;NumberOfElements=1029;Type=0;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;;DataType=100080000000000100094008000355363400010000000000000000000000000000;DisableOnOverflowUnderflow=FALSE"IO Module Clock 0ResourceName=IO Module Clock 0;TopSignalConnect=LvFpgaIoModClipClock0;ClockSignalName=LvFpgaIoModClipClock0;MinFreq=50000.000000;MaxFreq=500000000.000000;VariableFreq=0;NomFreq=50000000.000000;PeakPeriodJitter=250.000000;MinDutyCycle=45.000000;MaxDutyCycle=55.000000;Accuracy=100.000000;RunTime=0;SpreadSpectrum=0;GenericDataHash=D41D8CD98F00B204E9800998ECF8427EIO Module IO EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module IO Enabled;0;ReadMethodType=boolIO Module Power EnabledNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Enabled;0;ReadMethodType=boolIO Module Power GoodNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Power Good;0;ReadMethodType=boolIO Module PresentNumberOfSyncRegistersForReadInProject=Auto;resource=/IO Module Status/IO Module Present;0;ReadMethodType=boolIO Module67eb725630b9df645d87b6ea53b730adIOModuleID:0x109374F2,Version:1.1.0,National Instruments::NI 5751,SyncClock:DSTARA40eb31d638a681f3bf7d5fac39d8634e52de049f325cfa1a32f3ebdd679fcbf49207c7e191aec536f8c72983cb67bc3aa80b7a2f59294e4f38713f732f2478fcb23f4a83f40a50f4d63de7f83d2bcb6cec8e84e96d2a10a3874661cdf6ad609a&lt;Array&gt;
&lt;Name&gt;Generics&lt;/Name&gt;
&lt;Dimsize&gt;0&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Generic&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Type&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Default value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Value&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;Description&lt;/Name&gt;
&lt;Val&gt;&lt;/Val&gt;
&lt;/String&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
&lt;Array&gt;
&lt;Name&gt;Clock Connections&lt;/Name&gt;
&lt;Dimsize&gt;2&lt;/Dimsize&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock200&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;205000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;195000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;200 MHz Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock Connection&lt;/Name&gt;
&lt;NumElts&gt;2&lt;/NumElts&gt;
&lt;Cluster&gt;
&lt;Name&gt;CLIP Clock&lt;/Name&gt;
&lt;NumElts&gt;5&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;LabVIEW name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;String&gt;
&lt;Name&gt;VHDL name&lt;/Name&gt;
&lt;Val&gt;Clock40&lt;/Val&gt;
&lt;/String&gt;
&lt;DBL&gt;
&lt;Name&gt;Max Freq&lt;/Name&gt;
&lt;Val&gt;45000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;DBL&gt;
&lt;Name&gt;Min Freq&lt;/Name&gt;
&lt;Val&gt;35000000.00000000000000&lt;/Val&gt;
&lt;/DBL&gt;
&lt;EW&gt;
&lt;Name&gt;Direction&lt;/Name&gt;
&lt;Choice&gt;ToCLIP&lt;/Choice&gt;
&lt;Choice&gt;FromCLIP&lt;/Choice&gt;
&lt;Choice&gt;Bidirectional&lt;/Choice&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/EW&gt;
&lt;/Cluster&gt;
&lt;Cluster&gt;
&lt;Name&gt;FPGA Clock&lt;/Name&gt;
&lt;NumElts&gt;3&lt;/NumElts&gt;
&lt;String&gt;
&lt;Name&gt;Name&lt;/Name&gt;
&lt;Val&gt;40 MHz Onboard Clock&lt;/Val&gt;
&lt;/String&gt;
&lt;Boolean&gt;
&lt;Name&gt;Use Top Clock&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;Boolean&gt;
&lt;Name&gt;Clock From CLIP&lt;/Name&gt;
&lt;Val&gt;0&lt;/Val&gt;
&lt;/Boolean&gt;
&lt;/Cluster&gt;
&lt;/Cluster&gt;
&lt;/Array&gt;
LineClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig2;0;ReadMethodType=bool;WriteMethodType=boolPixelClockArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig1;0;ReadMethodType=bool;WriteMethodType=boolPll UnlockedNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/Pll Unlocked;0;ReadMethodType=boolPXI_Clk10NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Clk10;0;ReadMethodType=boolPXI_StarNumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Star;0;ReadMethodType=boolPXI_Trig3ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig3;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig4ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig4;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig5ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig5;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig6ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig6;0;ReadMethodType=bool;WriteMethodType=boolPXI_Trig7ArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig7;0;ReadMethodType=bool;WriteMethodType=boolPXIe_DStarBNumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarB;0;ReadMethodType=boolPXIe_DStarCArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXIe/PXIe_DStarC;0;ReadMethodType=bool;WriteMethodType=boolPXIe_Sync100NumberOfSyncRegistersForReadInProject=0;resource=/PXIe/PXIe_Sync100;0;ReadMethodType=boolPXIe-7962R/RioClk40/falsefalseFPGA_EXECUTION_MODEFPGA_TARGETFPGA_TARGET_CLASSPXIE_7962RFPGA_TARGET_FAMILYVIRTEX5TARGET_TYPEFPGASample Clock SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/Sample Clock Select;0;WriteMethodType=U8SPI AddressArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Address;0;WriteMethodType=U16SPI Device SelectArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Device Select;0;WriteMethodType=U8SPI IdleNumberOfSyncRegistersForReadInProject=0;resource=/IO Module/SignalList/SPI Idle;0;ReadMethodType=boolSPI Write DataArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write Data;0;WriteMethodType=U16SPI WriteArbitrationForOutputData=NeverArbitrate;NumberOfSyncRegistersForOutputData=0;resource=/IO Module/SignalList/SPI Write;0;WriteMethodType=boolToHostCh1FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh1FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"ToHostCh2FIFO"ControlLogic=0;NumberOfElements=8191;Type=2;ReadArbs=Arbitrate if Multiple Requestors Only;ElementsPerRead=1;WriteArbs=Arbitrate if Multiple Requestors Only;ElementsPerWrite=1;Implementation=2;ToHostCh2FIFO;DataType=1000800000000001000940070003553332000100000000000000000000;DisableOnOverflowUnderflow=FALSE"TriggerInArbitrationForOutputData=NeverArbitrate;ArbitrationForOutputEnable=NeverArbitrate;NumberOfSyncRegistersForOutputData=1;NumberOfSyncRegistersForOutputEnable=1;NumberOfSyncRegistersForReadInProject=Auto;resource=/PXI/PXI_Trig0;0;ReadMethodType=bool;WriteMethodType=bool</Property>
			</Item>
			<Item Name="Dependencies" Type="Dependencies">
				<Item Name="Floor_Negative16.vi" Type="VI" URL="../SubVIs/Floor_Negative16.vi"/>
				<Item Name="PixelClockU32.vi" Type="VI" URL="../../Common FPGA VIs/PixelClockU32.vi"/>
				<Item Name="niFpgaSctlEmulationGetInTimedLoop.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationGetInTimedLoop.vi"/>
				<Item Name="niFpgaSetErrorForExecOnDevCompSimple.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/errors/niFpgaSetErrorForExecOnDevCompSimple.vi"/>
				<Item Name="niFpgaGetScratchAppInstance.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/eio/common/niFpgaGetScratchAppInstance.vi"/>
				<Item Name="nirviEmuReportErrorAndStop.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/eio/common/nirviEmuReportErrorAndStop.vi"/>
				<Item Name="niFpgaSctlEmulationClkInfo.ctl" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationClkInfo.ctl"/>
				<Item Name="niFpgaSctlEmulationSchedulerRegClks.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationSchedulerRegClks.vi"/>
				<Item Name="nirviTagForDefaultClock.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/ClientSDK/Core/TimingSources/Configuration/Public/nirviTagForDefaultClock.vi"/>
				<Item Name="niFpgaSctlEmulationConstants.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationConstants.vi"/>
				<Item Name="niFpgaGenCallStack.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/errors/niFpgaGenCallStack.vi"/>
				<Item Name="nirviFillInErrorInfo.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/errors/nirviFillInErrorInfo.vi"/>
				<Item Name="niFpgaSctlEmulationIdMgrCmd.ctl" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationIdMgrCmd.ctl"/>
				<Item Name="niFpgaSctlEmulationIdMgr.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationIdMgr.vi"/>
				<Item Name="niFpgaSctlEmulationSchedulerHandleRollover.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationSchedulerHandleRollover.vi"/>
				<Item Name="nirvimemoryEmulationManagerCacheLock_Operations.ctl" Type="VI" URL="/&lt;vilib&gt;/rvi/Memory/Memory_Emulation/nirvimemoryEmulationManagerCacheLock_Operations.ctl"/>
				<Item Name="nirvimemoryEmulationManagerCacheLock.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Memory/Memory_Emulation/nirvimemoryEmulationManagerCacheLock.vi"/>
				<Item Name="nirvimemoryEmulationManagerCache_ReleaseExclusive.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Memory/Memory_Emulation/nirvimemoryEmulationManagerCache_ReleaseExclusive.vi"/>
				<Item Name="nirvimemoryEmulationManagerCache_Operations.ctl" Type="VI" URL="/&lt;vilib&gt;/rvi/Memory/Memory_Emulation/nirvimemoryEmulationManagerCache_Operations.ctl"/>
				<Item Name="nirvimemoryEmulationManagerCache.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Memory/Memory_Emulation/nirvimemoryEmulationManagerCache.vi"/>
				<Item Name="nirvimemoryEmulationManagerCache_GetValue.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Memory/Memory_Emulation/nirvimemoryEmulationManagerCache_GetValue.vi"/>
				<Item Name="nirvimemoryEmulationManagerCache_MakeExclusive.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Memory/Memory_Emulation/nirvimemoryEmulationManagerCache_MakeExclusive.vi"/>
				<Item Name="nirvimemoryEmulationManagerCache_SetValue.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Memory/Memory_Emulation/nirvimemoryEmulationManagerCache_SetValue.vi"/>
				<Item Name="niFpgaSctlEmulationFifoFullMgr.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationFifoFullMgr.vi"/>
				<Item Name="niFpgaSctlEmulationSharedResTypes.ctl" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationSharedResTypes.ctl"/>
				<Item Name="niFpgaSctlEmulationSharedResource.ctl" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationSharedResource.ctl"/>
				<Item Name="niFpgaSctlEmulationSharedResMgrCmd.ctl" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationSharedResMgrCmd.ctl"/>
				<Item Name="niFpgaSctlEmulationResourceMgr.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationResourceMgr.vi"/>
				<Item Name="nirviReportUnexpectedCaseInternalErrorHelper.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/errors/nirviReportUnexpectedCaseInternalErrorHelper.vi"/>
				<Item Name="nirviReportUnexpectedCaseInternalError (String).vi" Type="VI" URL="/&lt;vilib&gt;/rvi/errors/nirviReportUnexpectedCaseInternalError (String).vi"/>
				<Item Name="niFpgaSctlEmulationSchedulerUnRegClks.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationSchedulerUnRegClks.vi"/>
				<Item Name="niFpgaSctlEmulationSchedulerGenSchedule.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationSchedulerGenSchedule.vi"/>
				<Item Name="niFpgaSctlEmulationSchedulerState.ctl" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationSchedulerState.ctl"/>
				<Item Name="niFpgaSctlEmulationSchedulerCommand.ctl" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationSchedulerCommand.ctl"/>
				<Item Name="niFpgaSctlEmulationScheduler.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationScheduler.vi"/>
				<Item Name="niFpgaSctlEmulationGlobalWrite.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationGlobalWrite.vi"/>
				<Item Name="niFpgaSctlEmulationRegisterWithScheduler.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaSctlEmulationRegisterWithScheduler.vi"/>
				<Item Name="niFpgaEmulationVisToLoad.vi" Type="VI" URL="/&lt;vilib&gt;/rvi/Emulation/niFpgaEmulationVisToLoad.vi"/>
			</Item>
			<Item Name="Build Specifications" Type="Build">
				<Item Name="ResonanceScanner_PXIe-7962R" Type="{F4C5E96F-7410-48A5-BB87-3559BC9B167F}">
					<Property Name="AllowEnableRemoval" Type="Bool">false</Property>
					<Property Name="BuildSpecDecription" Type="Str"></Property>
					<Property Name="BuildSpecName" Type="Str">ResonanceScanner_PXIe-7962R</Property>
					<Property Name="Comp.BitfileName" Type="Str">ResonanceScanner_PXIe-7962R.lvbitx</Property>
					<Property Name="Comp.CustomXilinxParameters" Type="Str"></Property>
					<Property Name="Comp.MaxFanout" Type="Int">-1</Property>
					<Property Name="Comp.RandomSeed" Type="Bool">false</Property>
					<Property Name="Comp.Version.Build" Type="Int">0</Property>
					<Property Name="Comp.Version.Fix" Type="Int">0</Property>
					<Property Name="Comp.Version.Major" Type="Int">1</Property>
					<Property Name="Comp.Version.Minor" Type="Int">0</Property>
					<Property Name="Comp.VersionAutoIncrement" Type="Bool">false</Property>
					<Property Name="Comp.Xilinx.DesignStrategy" Type="Str">balanced</Property>
					<Property Name="Comp.Xilinx.MapEffort" Type="Str">high(timing)</Property>
					<Property Name="Comp.Xilinx.ParEffort" Type="Str">standard</Property>
					<Property Name="Comp.Xilinx.SynthEffort" Type="Str">normal</Property>
					<Property Name="Comp.Xilinx.SynthGoal" Type="Str">speed</Property>
					<Property Name="Comp.Xilinx.UseRecommended" Type="Bool">true</Property>
					<Property Name="DefaultBuildSpec" Type="Bool">true</Property>
					<Property Name="DestinationDirectory" Type="Path">/C/Scope/fpga_labview/FPGA Bitfiles</Property>
					<Property Name="ProjectPath" Type="Path">/C/Scope-Resonance/fpga_labview/ResonanceScanner/ResonanceScanner.lvproj</Property>
					<Property Name="RelativePath" Type="Bool">false</Property>
					<Property Name="RunWhenLoaded" Type="Bool">false</Property>
					<Property Name="SupportDownload" Type="Bool">true</Property>
					<Property Name="SupportResourceEstimation" Type="Bool">true</Property>
					<Property Name="TargetName" Type="Str">FPGA Target</Property>
					<Property Name="TopLevelVI" Type="Ref">/My Computer/FPGA Target/ResonanceScanner.vi</Property>
				</Item>
			</Item>
		</Item>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
			</Item>
			<Item Name="Build_Scatter_Data.vi" Type="VI" URL="../SubVIs/Build_Scatter_Data.vi"/>
			<Item Name="NiFpgaLv.dll" Type="Document" URL="NiFpgaLv.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
