<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="16008000">
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
		<Item Name="P Loop.vi" Type="VI" URL="../P Loop.vi"/>
		<Item Name="Sensitivity Test.vi" Type="VI" URL="../Sensitivity Test.vi"/>
		<Item Name="Test Move Straight.vi" Type="VI" URL="../Test Move Straight.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="Check Boolean Array.vi" Type="VI" URL="../../2017 Lelantos/Check Boolean Array.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
