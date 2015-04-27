<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="!stavebnice03">
<packages>
<package name="16F1503">
<wire x1="4.305" y1="-1.9" x2="-4.305" y2="-1.9" width="0.2032" layer="51"/>
<wire x1="-4.305" y1="-1.9" x2="-4.305" y2="-1.4" width="0.2032" layer="51"/>
<wire x1="-4.305" y1="-1.4" x2="-4.305" y2="1.9" width="0.2032" layer="51"/>
<wire x1="4.305" y1="-1.4" x2="-4.305" y2="-1.4" width="0.2032" layer="51"/>
<wire x1="4.305" y1="1.9" x2="4.305" y2="-1.4" width="0.2032" layer="51"/>
<wire x1="4.305" y1="-1.4" x2="4.305" y2="-1.9" width="0.2032" layer="51"/>
<wire x1="-4.305" y1="1.9" x2="4.305" y2="1.9" width="0.2032" layer="51"/>
<smd name="2" x="-2.54" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="13" x="-2.54" y="2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="1" x="-3.81" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="3" x="-1.27" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="4" x="0" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="14" x="-3.81" y="2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="12" x="-1.27" y="2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="11" x="0" y="2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="6" x="2.54" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="9" x="2.54" y="2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="5" x="1.27" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="7" x="3.81" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="10" x="1.27" y="2.6" dx="0.6" dy="2.2" layer="1"/>
<smd name="8" x="3.81" y="2.6" dx="0.6" dy="2.2" layer="1"/>
<text x="-4.572" y="-1.905" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="5.842" y="-1.905" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-4.0551" y1="-3.1001" x2="-3.5649" y2="-2" layer="51"/>
<rectangle x1="-2.7851" y1="-3.1001" x2="-2.2949" y2="-2" layer="51"/>
<rectangle x1="-1.5151" y1="-3.1001" x2="-1.0249" y2="-2" layer="51"/>
<rectangle x1="-0.2451" y1="-3.1001" x2="0.2451" y2="-2" layer="51"/>
<rectangle x1="-0.2451" y1="2" x2="0.2451" y2="3.1001" layer="51"/>
<rectangle x1="-1.5151" y1="2" x2="-1.0249" y2="3.1001" layer="51"/>
<rectangle x1="-2.7851" y1="2" x2="-2.2949" y2="3.1001" layer="51"/>
<rectangle x1="-4.0551" y1="2" x2="-3.5649" y2="3.1001" layer="51"/>
<rectangle x1="1.0249" y1="-3.1001" x2="1.5151" y2="-2" layer="51"/>
<rectangle x1="2.2949" y1="-3.1001" x2="2.7851" y2="-2" layer="51"/>
<rectangle x1="3.5649" y1="-3.1001" x2="4.0551" y2="-2" layer="51"/>
<rectangle x1="3.5649" y1="2" x2="4.0551" y2="3.1001" layer="51"/>
<rectangle x1="2.2949" y1="2" x2="2.7851" y2="3.1001" layer="51"/>
<rectangle x1="1.0249" y1="2" x2="1.5151" y2="3.1001" layer="51"/>
</package>
<package name="SO14">
<wire x1="4.064" y1="1.9558" x2="-4.064" y2="1.9558" width="0.1524" layer="21"/>
<wire x1="4.064" y1="-1.9558" x2="4.445" y2="-1.5748" width="0.1524" layer="21" curve="90"/>
<wire x1="-4.445" y1="1.5748" x2="-4.064" y2="1.9558" width="0.1524" layer="21" curve="-90"/>
<wire x1="4.064" y1="1.9558" x2="4.445" y2="1.5748" width="0.1524" layer="21" curve="-90"/>
<wire x1="-4.445" y1="-1.5748" x2="-4.064" y2="-1.9558" width="0.1524" layer="21" curve="90"/>
<wire x1="-4.064" y1="-1.9558" x2="4.064" y2="-1.9558" width="0.1524" layer="21"/>
<wire x1="4.445" y1="-1.5748" x2="4.445" y2="1.5748" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="1.5748" x2="-4.445" y2="0.508" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="0.508" x2="-4.445" y2="-0.508" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="-0.508" x2="-4.445" y2="-1.5748" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="0.508" x2="-4.445" y2="-0.508" width="0.1524" layer="21" curve="-180"/>
<wire x1="-4.445" y1="-1.6002" x2="4.445" y2="-1.6002" width="0.1524" layer="21"/>
<smd name="1" x="-3.81" y="-3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="14" x="-3.81" y="3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="2" x="-2.54" y="-3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="3" x="-1.27" y="-3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="13" x="-2.54" y="3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="12" x="-1.27" y="3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="4" x="0" y="-3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="11" x="0" y="3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="5" x="1.27" y="-3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="6" x="2.54" y="-3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="10" x="1.27" y="3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="9" x="2.54" y="3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="7" x="3.81" y="-3.0734" dx="0.6604" dy="2.032" layer="1"/>
<smd name="8" x="3.81" y="3.0734" dx="0.6604" dy="2.032" layer="1"/>
<text x="-3.556" y="-0.635" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-4.699" y="-1.778" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<rectangle x1="-0.254" y1="1.9558" x2="0.254" y2="3.0988" layer="51"/>
<rectangle x1="-4.064" y1="-3.0988" x2="-3.556" y2="-1.9558" layer="51"/>
<rectangle x1="-2.794" y1="-3.0988" x2="-2.286" y2="-1.9558" layer="51"/>
<rectangle x1="-1.524" y1="-3.0734" x2="-1.016" y2="-1.9304" layer="51"/>
<rectangle x1="-0.254" y1="-3.0988" x2="0.254" y2="-1.9558" layer="51"/>
<rectangle x1="-1.524" y1="1.9558" x2="-1.016" y2="3.0988" layer="51"/>
<rectangle x1="-2.794" y1="1.9558" x2="-2.286" y2="3.0988" layer="51"/>
<rectangle x1="-4.064" y1="1.9558" x2="-3.556" y2="3.0988" layer="51"/>
<rectangle x1="1.016" y1="1.9558" x2="1.524" y2="3.0988" layer="51"/>
<rectangle x1="2.286" y1="1.9558" x2="2.794" y2="3.0988" layer="51"/>
<rectangle x1="3.556" y1="1.9558" x2="4.064" y2="3.0988" layer="51"/>
<rectangle x1="1.016" y1="-3.0988" x2="1.524" y2="-1.9558" layer="51"/>
<rectangle x1="2.286" y1="-3.0988" x2="2.794" y2="-1.9558" layer="51"/>
<rectangle x1="3.556" y1="-3.0988" x2="4.064" y2="-1.9558" layer="51"/>
</package>
<package name="JUMPER4">
<wire x1="3.556" y1="1.524" x2="1.524" y2="1.524" width="0.1524" layer="21"/>
<wire x1="1.27" y1="1.27" x2="1.524" y2="1.524" width="0.1524" layer="21"/>
<wire x1="3.556" y1="-1.016" x2="1.524" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.762" x2="1.524" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.762" x2="1.27" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.556" y1="-1.016" x2="3.81" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="3.556" y1="1.524" x2="3.81" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.81" y1="1.27" x2="3.81" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.762" x2="-1.27" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.762" x2="-1.016" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="-1.016" x2="-1.27" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-0.762" x2="-3.556" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-0.762" x2="-3.81" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.016" y1="-1.016" x2="-1.016" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="1.016" y1="-1.016" x2="1.27" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="-1.016" x2="-3.556" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="1.016" y1="1.524" x2="-1.016" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="1.524" x2="-3.556" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="1.27" x2="-1.016" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="1.27" x2="-3.556" y2="1.524" width="0.1524" layer="21"/>
<wire x1="1.016" y1="1.524" x2="1.27" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="1.524" x2="-1.27" y2="1.27" width="0.1524" layer="21"/>
<wire x1="2.54" y1="2.032" x2="2.54" y2="6.985" width="0.6604" layer="21"/>
<wire x1="0" y1="2.032" x2="0" y2="6.985" width="0.6604" layer="21"/>
<wire x1="-2.54" y1="2.032" x2="-2.54" y2="6.985" width="0.6604" layer="21"/>
<pad name="1" x="-2.54" y="-2.54" drill="0.9144" shape="long" rot="R90"/>
<pad name="2" x="0" y="-2.54" drill="0.9144" shape="long" rot="R90"/>
<pad name="3" x="2.54" y="-2.54" drill="0.9144" shape="long" rot="R90"/>
<text x="-4.191" y="-1.016" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="-2.794" y="0" size="0.9906" layer="21" ratio="12">1</text>
<text x="-0.381" y="0" size="0.9906" layer="21" ratio="12">2</text>
<text x="2.032" y="0" size="0.9906" layer="21" ratio="12">3</text>
<text x="9.271" y="-1.143" size="1.27" layer="27" ratio="10" rot="R90">&gt;VALUE</text>
<rectangle x1="2.2352" y1="1.524" x2="2.8448" y2="2.032" layer="21"/>
<rectangle x1="2.2352" y1="-1.524" x2="2.8448" y2="-1.016" layer="21"/>
<rectangle x1="-0.3048" y1="-1.524" x2="0.3048" y2="-1.016" layer="21"/>
<rectangle x1="-2.8448" y1="-1.524" x2="-2.2352" y2="-1.016" layer="21"/>
<rectangle x1="2.2352" y1="-2.794" x2="2.8448" y2="-1.524" layer="51"/>
<rectangle x1="-0.3048" y1="-2.794" x2="0.3048" y2="-1.524" layer="51"/>
<rectangle x1="-2.8448" y1="-2.794" x2="-2.2352" y2="-1.524" layer="51"/>
<rectangle x1="-0.3048" y1="1.524" x2="0.3048" y2="2.032" layer="21"/>
<rectangle x1="-2.8448" y1="1.524" x2="-2.2352" y2="2.032" layer="21"/>
<wire x1="6.096" y1="1.524" x2="4.064" y2="1.524" width="0.1524" layer="21"/>
<wire x1="3.81" y1="1.27" x2="4.064" y2="1.524" width="0.1524" layer="21"/>
<wire x1="6.096" y1="-1.016" x2="4.064" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-0.762" x2="4.064" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="6.096" y1="-1.016" x2="6.35" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="6.096" y1="1.524" x2="6.35" y2="1.27" width="0.1524" layer="21"/>
<wire x1="6.35" y1="1.27" x2="6.35" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="5.08" y1="2.032" x2="5.08" y2="6.985" width="0.6604" layer="21"/>
<pad name="4" x="5.08" y="-2.54" drill="0.9144" shape="long" rot="R90"/>
<text x="4.572" y="0" size="0.9906" layer="21" ratio="12">4</text>
<rectangle x1="4.7752" y1="1.524" x2="5.3848" y2="2.032" layer="21"/>
<rectangle x1="4.7752" y1="-1.524" x2="5.3848" y2="-1.016" layer="21"/>
<rectangle x1="4.7752" y1="-2.794" x2="5.3848" y2="-1.524" layer="51"/>
</package>
<package name="JUMPER4F">
<wire x1="1.27" y1="2.032" x2="1.27" y2="6.985" width="0.6604" layer="21"/>
<wire x1="-1.27" y1="2.032" x2="-1.27" y2="6.985" width="0.6604" layer="21"/>
<wire x1="-3.81" y1="2.032" x2="-3.81" y2="6.985" width="0.6604" layer="21"/>
<pad name="1" x="-3.81" y="-2.54" drill="0.9144" shape="long" rot="R90"/>
<pad name="2" x="-1.27" y="-2.54" drill="0.9144" shape="long" rot="R90"/>
<pad name="3" x="1.27" y="-2.54" drill="0.9144" shape="long" rot="R90"/>
<text x="-5.461" y="-1.016" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="-4.064" y="0" size="0.9906" layer="21" ratio="12">1</text>
<text x="-1.651" y="0" size="0.9906" layer="21" ratio="12">2</text>
<text x="0.762" y="0" size="0.9906" layer="21" ratio="12">3</text>
<text x="8.001" y="-1.143" size="1.27" layer="27" ratio="10" rot="R90">&gt;VALUE</text>
<rectangle x1="0.9652" y1="1.524" x2="1.5748" y2="2.032" layer="21"/>
<rectangle x1="0.9652" y1="-1.524" x2="1.5748" y2="-1.016" layer="21"/>
<rectangle x1="-1.5748" y1="-1.524" x2="-0.9652" y2="-1.016" layer="21"/>
<rectangle x1="-4.1148" y1="-1.524" x2="-3.5052" y2="-1.016" layer="21"/>
<rectangle x1="0.9652" y1="-2.794" x2="1.5748" y2="-1.524" layer="51"/>
<rectangle x1="-1.5748" y1="-2.794" x2="-0.9652" y2="-1.524" layer="51"/>
<rectangle x1="-4.1148" y1="-2.794" x2="-3.5052" y2="-1.524" layer="51"/>
<rectangle x1="-1.5748" y1="1.524" x2="-0.9652" y2="2.032" layer="21"/>
<rectangle x1="-4.1148" y1="1.524" x2="-3.5052" y2="2.032" layer="21"/>
<wire x1="3.81" y1="2.032" x2="3.81" y2="6.985" width="0.6604" layer="21"/>
<pad name="4" x="3.81" y="-2.54" drill="0.9144" shape="long" rot="R90"/>
<text x="3.302" y="0" size="0.9906" layer="21" ratio="12">4</text>
<rectangle x1="3.5052" y1="-1.524" x2="4.1148" y2="-1.016" layer="21"/>
<rectangle x1="3.5052" y1="-2.794" x2="4.1148" y2="-1.524" layer="51"/>
<wire x1="-5.08" y1="-1.27" x2="5.08" y2="-1.27" width="0.127" layer="21"/>
<wire x1="5.08" y1="-1.27" x2="5.08" y2="7.62" width="0.127" layer="21"/>
<wire x1="5.08" y1="7.62" x2="-5.08" y2="7.62" width="0.127" layer="21"/>
<wire x1="-5.08" y1="7.62" x2="-5.08" y2="-1.27" width="0.127" layer="21"/>
</package>
<package name="JUMPER6M">
<wire x1="-7.62" y1="-1.016" x2="-7.62" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="1.27" x2="-2.286" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.016" x2="-2.286" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-1.27" x2="-2.286" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.016" x2="-2.286" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-1.27" x2="0" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="1.27" x2="0" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="1.27" x2="-2.54" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="1.27" x2="-4.826" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="1.016" x2="-4.826" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-5.334" y1="1.27" x2="-5.08" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="1.016" x2="-7.366" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-5.334" y1="1.27" x2="-7.366" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="-1.27" x2="-2.54" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-1.016" x2="-4.826" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.334" y1="-1.27" x2="-5.08" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="-1.016" x2="-7.366" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="-1.27" x2="-4.826" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.334" y1="-1.27" x2="-7.366" y2="-1.27" width="0.1524" layer="21"/>
<pad name="1" x="-6.35" y="0" drill="0.9144" shape="long" rot="R90"/>
<pad name="2" x="-3.81" y="0" drill="0.9144" shape="long" rot="R90"/>
<pad name="3" x="-1.27" y="0" drill="0.9144" shape="long" rot="R90"/>
<text x="-7.366" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.35" y="-2.667" size="0.9906" layer="21" ratio="12">1</text>
<text x="-3.81" y="-2.667" size="0.9906" layer="21" ratio="12">2</text>
<text x="-1.27" y="-2.667" size="0.9906" layer="21" ratio="12">3</text>
<text x="-7.366" y="-4.318" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-6.6548" y1="-0.3048" x2="-6.0452" y2="0.3048" layer="51"/>
<rectangle x1="-4.1148" y1="-0.3048" x2="-3.5052" y2="0.3048" layer="51"/>
<rectangle x1="-1.5748" y1="-0.3048" x2="-0.9652" y2="0.3048" layer="51"/>
<wire x1="7.366" y1="1.27" x2="5.334" y2="1.27" width="0.1524" layer="21"/>
<wire x1="5.08" y1="1.016" x2="5.334" y2="1.27" width="0.1524" layer="21"/>
<wire x1="7.366" y1="-1.27" x2="5.334" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-1.016" x2="5.334" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="7.366" y1="-1.27" x2="7.62" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="7.366" y1="1.27" x2="7.62" y2="1.016" width="0.1524" layer="21"/>
<wire x1="7.62" y1="1.016" x2="7.62" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="4.826" y1="1.27" x2="5.08" y2="1.016" width="0.1524" layer="21"/>
<wire x1="4.826" y1="1.27" x2="2.794" y2="1.27" width="0.1524" layer="21"/>
<wire x1="2.54" y1="1.016" x2="2.794" y2="1.27" width="0.1524" layer="21"/>
<wire x1="2.286" y1="1.27" x2="2.54" y2="1.016" width="0.1524" layer="21"/>
<wire x1="0" y1="1.016" x2="0.254" y2="1.27" width="0.1524" layer="21"/>
<wire x1="2.286" y1="1.27" x2="0.254" y2="1.27" width="0.1524" layer="21"/>
<wire x1="4.826" y1="-1.27" x2="5.08" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.016" x2="2.794" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.286" y1="-1.27" x2="2.54" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.016" x2="0.254" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.826" y1="-1.27" x2="2.794" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.286" y1="-1.27" x2="0.254" y2="-1.27" width="0.1524" layer="21"/>
<pad name="4" x="1.27" y="0" drill="0.9144" shape="long" rot="R90"/>
<pad name="5" x="3.81" y="0" drill="0.9144" shape="long" rot="R90"/>
<pad name="6" x="6.35" y="0" drill="0.9144" shape="long" rot="R90"/>
<text x="1.27" y="-2.667" size="0.9906" layer="21" ratio="12">4</text>
<text x="3.81" y="-2.667" size="0.9906" layer="21" ratio="12">5</text>
<text x="6.35" y="-2.667" size="0.9906" layer="21" ratio="12">6</text>
<rectangle x1="0.9652" y1="-0.3048" x2="1.5748" y2="0.3048" layer="51"/>
<rectangle x1="3.5052" y1="-0.3048" x2="4.1148" y2="0.3048" layer="51"/>
<rectangle x1="6.0452" y1="-0.3048" x2="6.6548" y2="0.3048" layer="51"/>
</package>
<package name="JUMPER4M">
<wire x1="-5.08" y1="-1.016" x2="-5.08" y2="1.016" width="0.1524" layer="21"/>
<wire x1="2.286" y1="1.27" x2="0.254" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0" y1="1.016" x2="0.254" y2="1.27" width="0.1524" layer="21"/>
<wire x1="2.286" y1="-1.27" x2="0.254" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.016" x2="0.254" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.286" y1="-1.27" x2="2.54" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.286" y1="1.27" x2="2.54" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="1.27" x2="0" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="1.27" x2="-2.286" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.016" x2="-2.286" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="1.27" x2="-2.54" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="1.016" x2="-4.826" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="1.27" x2="-4.826" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-1.27" x2="0" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.016" x2="-2.286" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="-1.27" x2="-2.54" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-1.016" x2="-4.826" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-1.27" x2="-2.286" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="-1.27" x2="-4.826" y2="-1.27" width="0.1524" layer="21"/>
<pad name="1" x="-3.81" y="0" drill="0.9144" shape="long" rot="R90"/>
<pad name="2" x="-1.27" y="0" drill="0.9144" shape="long" rot="R90"/>
<pad name="3" x="1.27" y="0" drill="0.9144" shape="long" rot="R90"/>
<text x="-4.826" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-2.667" size="0.9906" layer="21" ratio="12">1</text>
<text x="-1.27" y="-2.667" size="0.9906" layer="21" ratio="12">2</text>
<text x="1.27" y="-2.667" size="0.9906" layer="21" ratio="12">3</text>
<text x="-4.826" y="-4.318" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-4.1148" y1="-0.3048" x2="-3.5052" y2="0.3048" layer="51"/>
<rectangle x1="-1.5748" y1="-0.3048" x2="-0.9652" y2="0.3048" layer="51"/>
<rectangle x1="0.9652" y1="-0.3048" x2="1.5748" y2="0.3048" layer="51"/>
<wire x1="4.826" y1="1.27" x2="5.08" y2="1.016" width="0.1524" layer="21"/>
<wire x1="2.54" y1="1.016" x2="2.794" y2="1.27" width="0.1524" layer="21"/>
<wire x1="4.826" y1="1.27" x2="2.794" y2="1.27" width="0.1524" layer="21"/>
<wire x1="4.826" y1="-1.27" x2="5.08" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.016" x2="2.794" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.826" y1="-1.27" x2="2.794" y2="-1.27" width="0.1524" layer="21"/>
<pad name="4" x="3.81" y="0" drill="0.9144" shape="long" rot="R90"/>
<text x="3.81" y="-2.667" size="0.9906" layer="21" ratio="12">4</text>
<rectangle x1="3.5052" y1="-0.3048" x2="4.1148" y2="0.3048" layer="51"/>
<wire x1="5.08" y1="-1.016" x2="5.08" y2="1.016" width="0.1524" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="16F1503">
<pin name="VCC" x="-20.32" y="12.7" length="middle" direction="pwr"/>
<pin name="RA5" x="-20.32" y="7.62" length="middle"/>
<pin name="RA4" x="-20.32" y="2.54" length="middle"/>
<pin name="MCLR/VPP/RA3" x="-20.32" y="-2.54" length="middle"/>
<pin name="RC5" x="-20.32" y="-7.62" length="middle"/>
<pin name="RC4" x="-20.32" y="-12.7" length="middle"/>
<pin name="RC3" x="-20.32" y="-17.78" length="middle"/>
<pin name="GND" x="20.32" y="12.7" length="middle" direction="pwr" rot="R180"/>
<pin name="RA0/ICSPDAT" x="20.32" y="7.62" length="middle" rot="R180"/>
<pin name="RA1/ICSPCLK" x="20.32" y="2.54" length="middle" rot="R180"/>
<pin name="RA2" x="20.32" y="-2.54" length="middle" rot="R180"/>
<pin name="RC0" x="20.32" y="-7.62" length="middle" rot="R180"/>
<pin name="RC1" x="20.32" y="-12.7" length="middle" rot="R180"/>
<pin name="RC2" x="20.32" y="-17.78" length="middle" rot="R180"/>
<wire x1="-15.24" y1="15.24" x2="15.24" y2="15.24" width="0.254" layer="94"/>
<wire x1="15.24" y1="15.24" x2="15.24" y2="-20.32" width="0.254" layer="94"/>
<wire x1="15.24" y1="-20.32" x2="-15.24" y2="-20.32" width="0.254" layer="94"/>
<wire x1="-15.24" y1="-20.32" x2="-15.24" y2="15.24" width="0.254" layer="94"/>
<text x="-15.24" y="-22.86" size="1.778" layer="94">16F1503</text>
</symbol>
<symbol name="4016">
<pin name="Y0" x="-5.08" y="7.62" length="middle"/>
<pin name="Z0" x="-5.08" y="2.54" length="middle"/>
<pin name="Z1" x="-5.08" y="-2.54" length="middle"/>
<pin name="Y1" x="-5.08" y="-7.62" length="middle"/>
<pin name="E1" x="-5.08" y="-12.7" length="middle"/>
<pin name="E2" x="-5.08" y="-17.78" length="middle"/>
<pin name="GND" x="-5.08" y="-22.86" length="middle"/>
<pin name="Y2" x="20.32" y="-22.86" length="middle" rot="R180"/>
<pin name="Z2" x="20.32" y="-17.78" length="middle" rot="R180"/>
<pin name="Z3" x="20.32" y="-12.7" length="middle" rot="R180"/>
<pin name="Y3" x="20.32" y="-7.62" length="middle" rot="R180"/>
<pin name="E3" x="20.32" y="-2.54" length="middle" rot="R180"/>
<pin name="E0" x="20.32" y="2.54" length="middle" rot="R180"/>
<pin name="VCC" x="20.32" y="7.62" length="middle" rot="R180"/>
<wire x1="0" y1="10.16" x2="15.24" y2="10.16" width="0.254" layer="94"/>
<wire x1="15.24" y1="10.16" x2="15.24" y2="-25.4" width="0.254" layer="94"/>
<wire x1="15.24" y1="-25.4" x2="0" y2="-25.4" width="0.254" layer="94"/>
<wire x1="0" y1="-25.4" x2="0" y2="10.16" width="0.254" layer="94"/>
<text x="0" y="-27.94" size="1.27" layer="94">4016</text>
</symbol>
<symbol name="JUMPER4">
<pin name="1" x="-2.54" y="7.62" length="middle"/>
<pin name="2" x="-2.54" y="2.54" length="middle"/>
<pin name="3" x="-2.54" y="-2.54" length="middle"/>
<pin name="4" x="-2.54" y="-7.62" length="middle"/>
<wire x1="2.54" y1="10.16" x2="5.08" y2="10.16" width="0.254" layer="94"/>
<wire x1="5.08" y1="10.16" x2="5.08" y2="7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="7.62" x2="5.08" y2="2.54" width="0.254" layer="94"/>
<wire x1="5.08" y1="2.54" x2="5.08" y2="-2.54" width="0.254" layer="94"/>
<wire x1="5.08" y1="-2.54" x2="5.08" y2="-7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="-7.62" x2="5.08" y2="-10.16" width="0.254" layer="94"/>
<wire x1="5.08" y1="-10.16" x2="2.54" y2="-10.16" width="0.254" layer="94"/>
<wire x1="2.54" y1="-10.16" x2="2.54" y2="10.16" width="0.254" layer="94"/>
<wire x1="5.08" y1="7.62" x2="12.7" y2="7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="2.54" x2="12.7" y2="2.54" width="0.254" layer="94"/>
<wire x1="5.08" y1="-2.54" x2="12.7" y2="-2.54" width="0.254" layer="94"/>
<wire x1="5.08" y1="-7.62" x2="12.7" y2="-7.62" width="0.254" layer="94"/>
</symbol>
<symbol name="JUMPER4F">
<pin name="1" x="-5.08" y="7.62" length="middle"/>
<pin name="2" x="-5.08" y="2.54" length="middle"/>
<pin name="3" x="-5.08" y="-2.54" length="middle"/>
<pin name="4" x="-5.08" y="-7.62" length="middle"/>
<wire x1="0" y1="-10.16" x2="0" y2="10.16" width="0.254" layer="94"/>
<wire x1="0" y1="10.16" x2="7.62" y2="10.16" width="0.254" layer="94"/>
<wire x1="7.62" y1="10.16" x2="7.62" y2="-10.16" width="0.254" layer="94"/>
<wire x1="7.62" y1="-10.16" x2="0" y2="-10.16" width="0.254" layer="94"/>
</symbol>
<symbol name="JUMPER6M">
<pin name="1" x="-7.62" y="12.7" length="middle"/>
<pin name="2" x="-7.62" y="7.62" length="middle"/>
<pin name="3" x="-7.62" y="2.54" length="middle"/>
<pin name="4" x="-7.62" y="-2.54" length="middle"/>
<wire x1="-2.54" y1="15.24" x2="0" y2="15.24" width="0.254" layer="94"/>
<wire x1="0" y1="15.24" x2="0" y2="12.7" width="0.254" layer="94"/>
<wire x1="0" y1="12.7" x2="0" y2="7.62" width="0.254" layer="94"/>
<wire x1="0" y1="7.62" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="-7.62" width="0.254" layer="94"/>
<wire x1="0" y1="-7.62" x2="0" y2="-12.7" width="0.254" layer="94"/>
<wire x1="0" y1="-12.7" x2="0" y2="-15.24" width="0.254" layer="94"/>
<wire x1="0" y1="-15.24" x2="-2.54" y2="-15.24" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-15.24" x2="-2.54" y2="15.24" width="0.254" layer="94"/>
<wire x1="0" y1="12.7" x2="7.62" y2="12.7" width="0.254" layer="94"/>
<wire x1="0" y1="7.62" x2="7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="7.62" y2="-2.54" width="0.254" layer="94"/>
<pin name="5" x="-7.62" y="-7.62" length="middle"/>
<pin name="6" x="-7.62" y="-12.7" length="middle"/>
<wire x1="0" y1="-7.62" x2="7.62" y2="-7.62" width="0.254" layer="94"/>
<wire x1="0" y1="-12.7" x2="7.62" y2="-12.7" width="0.254" layer="94"/>
</symbol>
<symbol name="JIMPER4M">
<pin name="1" x="-7.62" y="7.62" length="middle"/>
<pin name="2" x="-7.62" y="2.54" length="middle"/>
<pin name="3" x="-7.62" y="-2.54" length="middle"/>
<pin name="4" x="-7.62" y="-7.62" length="middle"/>
<wire x1="-2.54" y1="10.16" x2="0" y2="10.16" width="0.254" layer="94"/>
<wire x1="0" y1="10.16" x2="0" y2="7.62" width="0.254" layer="94"/>
<wire x1="0" y1="7.62" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="-7.62" width="0.254" layer="94"/>
<wire x1="0" y1="-7.62" x2="0" y2="-10.16" width="0.254" layer="94"/>
<wire x1="0" y1="-10.16" x2="-2.54" y2="-10.16" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-10.16" x2="-2.54" y2="10.16" width="0.254" layer="94"/>
<wire x1="0" y1="7.62" x2="7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="7.62" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-7.62" x2="7.62" y2="-7.62" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="16F1503">
<gates>
<gate name="G$1" symbol="16F1503" x="45.72" y="5.08"/>
</gates>
<devices>
<device name="" package="16F1503">
<connects>
<connect gate="G$1" pin="GND" pad="14"/>
<connect gate="G$1" pin="MCLR/VPP/RA3" pad="4"/>
<connect gate="G$1" pin="RA0/ICSPDAT" pad="13"/>
<connect gate="G$1" pin="RA1/ICSPCLK" pad="12"/>
<connect gate="G$1" pin="RA2" pad="11"/>
<connect gate="G$1" pin="RA4" pad="3"/>
<connect gate="G$1" pin="RA5" pad="2"/>
<connect gate="G$1" pin="RC0" pad="10"/>
<connect gate="G$1" pin="RC1" pad="9"/>
<connect gate="G$1" pin="RC2" pad="8"/>
<connect gate="G$1" pin="RC3" pad="7"/>
<connect gate="G$1" pin="RC4" pad="6"/>
<connect gate="G$1" pin="RC5" pad="5"/>
<connect gate="G$1" pin="VCC" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="4016">
<gates>
<gate name="G$1" symbol="4016" x="-7.62" y="5.08"/>
</gates>
<devices>
<device name="" package="SO14">
<connects>
<connect gate="G$1" pin="E0" pad="13"/>
<connect gate="G$1" pin="E1" pad="5"/>
<connect gate="G$1" pin="E2" pad="6"/>
<connect gate="G$1" pin="E3" pad="12"/>
<connect gate="G$1" pin="GND" pad="7"/>
<connect gate="G$1" pin="VCC" pad="14"/>
<connect gate="G$1" pin="Y0" pad="1"/>
<connect gate="G$1" pin="Y1" pad="4"/>
<connect gate="G$1" pin="Y2" pad="8"/>
<connect gate="G$1" pin="Y3" pad="11"/>
<connect gate="G$1" pin="Z0" pad="2"/>
<connect gate="G$1" pin="Z1" pad="3"/>
<connect gate="G$1" pin="Z2" pad="9"/>
<connect gate="G$1" pin="Z3" pad="10"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="JUMPER4">
<gates>
<gate name="G$1" symbol="JUMPER4" x="2.54" y="0"/>
</gates>
<devices>
<device name="" package="JUMPER4">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="JUMPER4F">
<gates>
<gate name="G$1" symbol="JUMPER4F" x="0" y="0"/>
</gates>
<devices>
<device name="" package="JUMPER4F">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="JUMPER6M">
<gates>
<gate name="G$1" symbol="JUMPER6M" x="0" y="0"/>
</gates>
<devices>
<device name="" package="JUMPER6M">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="JUMPER4M">
<gates>
<gate name="G$1" symbol="JIMPER4M" x="0" y="0"/>
</gates>
<devices>
<device name="" package="JUMPER4M">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U$1" library="!stavebnice03" deviceset="16F1503" device=""/>
<part name="U$2" library="!stavebnice03" deviceset="4016" device=""/>
<part name="U$3" library="!stavebnice03" deviceset="JUMPER4" device=""/>
<part name="U$4" library="!stavebnice03" deviceset="JUMPER4F" device=""/>
<part name="U$6" library="!stavebnice03" deviceset="JUMPER6M" device=""/>
<part name="U$5" library="!stavebnice03" deviceset="JUMPER4M" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$1" gate="G$1" x="144.78" y="86.36"/>
<instance part="U$2" gate="G$1" x="139.7" y="137.16"/>
<instance part="U$3" gate="G$1" x="198.12" y="101.6"/>
<instance part="U$4" gate="G$1" x="86.36" y="101.6" rot="MR0"/>
<instance part="U$6" gate="G$1" x="160.02" y="25.4" rot="MR270"/>
<instance part="U$5" gate="G$1" x="124.46" y="27.94" rot="R270"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="4"/>
<wire x1="91.44" y1="93.98" x2="101.6" y2="93.98" width="0.1524" layer="91"/>
<wire x1="101.6" y1="93.98" x2="101.6" y2="106.68" width="0.1524" layer="91"/>
<wire x1="101.6" y1="106.68" x2="134.62" y2="106.68" width="0.1524" layer="91"/>
<wire x1="134.62" y1="106.68" x2="165.1" y2="106.68" width="0.1524" layer="91"/>
<wire x1="165.1" y1="106.68" x2="175.26" y2="106.68" width="0.1524" layer="91"/>
<wire x1="175.26" y1="106.68" x2="187.96" y2="106.68" width="0.1524" layer="91"/>
<wire x1="187.96" y1="106.68" x2="187.96" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="4"/>
<wire x1="187.96" y1="93.98" x2="195.58" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="GND"/>
<wire x1="165.1" y1="99.06" x2="165.1" y2="106.68" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="GND"/>
<wire x1="134.62" y1="114.3" x2="134.62" y2="106.68" width="0.1524" layer="91"/>
<junction x="134.62" y="106.68"/>
<junction x="165.1" y="106.68"/>
<pinref part="U$6" gate="G$1" pin="3"/>
<wire x1="157.48" y1="33.02" x2="157.48" y2="53.34" width="0.1524" layer="91"/>
<wire x1="157.48" y1="53.34" x2="175.26" y2="53.34" width="0.1524" layer="91"/>
<wire x1="175.26" y1="53.34" x2="175.26" y2="106.68" width="0.1524" layer="91"/>
<junction x="175.26" y="106.68"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="1"/>
<pinref part="U$3" gate="G$1" pin="1"/>
<wire x1="91.44" y1="109.22" x2="111.76" y2="109.22" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="VCC"/>
<wire x1="111.76" y1="109.22" x2="121.92" y2="109.22" width="0.1524" layer="91"/>
<wire x1="121.92" y1="109.22" x2="167.64" y2="109.22" width="0.1524" layer="91"/>
<wire x1="167.64" y1="109.22" x2="195.58" y2="109.22" width="0.1524" layer="91"/>
<wire x1="160.02" y1="144.78" x2="167.64" y2="144.78" width="0.1524" layer="91"/>
<wire x1="167.64" y1="144.78" x2="167.64" y2="109.22" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="VCC"/>
<wire x1="124.46" y1="99.06" x2="121.92" y2="99.06" width="0.1524" layer="91"/>
<wire x1="121.92" y1="99.06" x2="121.92" y2="109.22" width="0.1524" layer="91"/>
<junction x="121.92" y="109.22"/>
<junction x="167.64" y="109.22"/>
<pinref part="U$6" gate="G$1" pin="2"/>
<wire x1="152.4" y1="33.02" x2="152.4" y2="43.18" width="0.1524" layer="91"/>
<wire x1="152.4" y1="43.18" x2="111.76" y2="43.18" width="0.1524" layer="91"/>
<wire x1="111.76" y1="43.18" x2="111.76" y2="109.22" width="0.1524" layer="91"/>
<junction x="111.76" y="109.22"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="RA5"/>
<wire x1="124.46" y1="93.98" x2="116.84" y2="93.98" width="0.1524" layer="91"/>
<wire x1="116.84" y1="93.98" x2="116.84" y2="119.38" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="E2"/>
<wire x1="116.84" y1="119.38" x2="134.62" y2="119.38" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="E1"/>
<wire x1="134.62" y1="124.46" x2="116.84" y2="124.46" width="0.1524" layer="91"/>
<wire x1="116.84" y1="124.46" x2="116.84" y2="119.38" width="0.1524" layer="91"/>
<junction x="116.84" y="119.38"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E3"/>
<wire x1="160.02" y1="134.62" x2="162.56" y2="134.62" width="0.1524" layer="91"/>
<wire x1="162.56" y1="134.62" x2="162.56" y2="139.7" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="E0"/>
<wire x1="162.56" y1="139.7" x2="160.02" y2="139.7" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="RA4"/>
<wire x1="124.46" y1="88.9" x2="114.3" y2="88.9" width="0.1524" layer="91"/>
<wire x1="114.3" y1="88.9" x2="114.3" y2="152.4" width="0.1524" layer="91"/>
<wire x1="114.3" y1="152.4" x2="162.56" y2="152.4" width="0.1524" layer="91"/>
<wire x1="162.56" y1="152.4" x2="162.56" y2="139.7" width="0.1524" layer="91"/>
<junction x="162.56" y="139.7"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<wire x1="172.72" y1="78.74" x2="172.72" y2="55.88" width="0.1524" layer="91"/>
<wire x1="172.72" y1="55.88" x2="119.38" y2="55.88" width="0.1524" layer="91"/>
<wire x1="119.38" y1="55.88" x2="119.38" y2="134.62" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="Z0"/>
<pinref part="U$2" gate="G$1" pin="Z1"/>
<wire x1="134.62" y1="139.7" x2="134.62" y2="134.62" width="0.1524" layer="91"/>
<junction x="134.62" y="134.62"/>
<wire x1="119.38" y1="134.62" x2="134.62" y2="134.62" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="RC0"/>
<wire x1="165.1" y1="78.74" x2="172.72" y2="78.74" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="Z2"/>
<pinref part="U$2" gate="G$1" pin="Z3"/>
<wire x1="160.02" y1="119.38" x2="160.02" y2="124.46" width="0.1524" layer="91"/>
<wire x1="180.34" y1="119.38" x2="180.34" y2="73.66" width="0.1524" layer="91"/>
<wire x1="160.02" y1="119.38" x2="180.34" y2="119.38" width="0.1524" layer="91"/>
<junction x="160.02" y="119.38"/>
<pinref part="U$1" gate="G$1" pin="RC1"/>
<wire x1="165.1" y1="73.66" x2="180.34" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="Y0"/>
<wire x1="134.62" y1="144.78" x2="93.98" y2="144.78" width="0.1524" layer="91"/>
<wire x1="93.98" y1="144.78" x2="93.98" y2="99.06" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="3"/>
<wire x1="93.98" y1="99.06" x2="91.44" y2="99.06" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="Y1"/>
<wire x1="134.62" y1="129.54" x2="129.54" y2="129.54" width="0.1524" layer="91"/>
<wire x1="129.54" y1="129.54" x2="129.54" y2="154.94" width="0.1524" layer="91"/>
<wire x1="129.54" y1="154.94" x2="193.04" y2="154.94" width="0.1524" layer="91"/>
<wire x1="193.04" y1="154.94" x2="193.04" y2="99.06" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="3"/>
<wire x1="193.04" y1="99.06" x2="195.58" y2="99.06" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="Y3"/>
<wire x1="160.02" y1="129.54" x2="172.72" y2="129.54" width="0.1524" layer="91"/>
<wire x1="172.72" y1="129.54" x2="172.72" y2="160.02" width="0.1524" layer="91"/>
<wire x1="172.72" y1="160.02" x2="96.52" y2="160.02" width="0.1524" layer="91"/>
<wire x1="96.52" y1="160.02" x2="96.52" y2="104.14" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="2"/>
<wire x1="96.52" y1="104.14" x2="91.44" y2="104.14" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="Y2"/>
<wire x1="160.02" y1="114.3" x2="190.5" y2="114.3" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="2"/>
<wire x1="195.58" y1="104.14" x2="190.5" y2="104.14" width="0.1524" layer="91"/>
<wire x1="190.5" y1="104.14" x2="190.5" y2="114.3" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="1"/>
<wire x1="147.32" y1="33.02" x2="147.32" y2="40.64" width="0.1524" layer="91"/>
<wire x1="147.32" y1="40.64" x2="106.68" y2="40.64" width="0.1524" layer="91"/>
<wire x1="106.68" y1="40.64" x2="106.68" y2="83.82" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="MCLR/VPP/RA3"/>
<wire x1="106.68" y1="83.82" x2="124.46" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="4"/>
<wire x1="162.56" y1="33.02" x2="162.56" y2="50.8" width="0.1524" layer="91"/>
<wire x1="162.56" y1="50.8" x2="167.64" y2="50.8" width="0.1524" layer="91"/>
<wire x1="167.64" y1="50.8" x2="167.64" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="RA0/ICSPDAT"/>
<wire x1="167.64" y1="93.98" x2="165.1" y2="93.98" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="RA1/ICSPCLK"/>
<wire x1="165.1" y1="88.9" x2="170.18" y2="88.9" width="0.1524" layer="91"/>
<wire x1="170.18" y1="88.9" x2="170.18" y2="40.64" width="0.1524" layer="91"/>
<pinref part="U$6" gate="G$1" pin="5"/>
<wire x1="170.18" y1="40.64" x2="167.64" y2="40.64" width="0.1524" layer="91"/>
<wire x1="167.64" y1="40.64" x2="167.64" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="RA2"/>
<wire x1="165.1" y1="83.82" x2="177.8" y2="83.82" width="0.1524" layer="91"/>
<wire x1="177.8" y1="83.82" x2="177.8" y2="38.1" width="0.1524" layer="91"/>
<pinref part="U$6" gate="G$1" pin="6"/>
<wire x1="177.8" y1="38.1" x2="172.72" y2="38.1" width="0.1524" layer="91"/>
<wire x1="172.72" y1="38.1" x2="172.72" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<wire x1="116.84" y1="35.56" x2="116.84" y2="78.74" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="RC5"/>
<wire x1="116.84" y1="78.74" x2="124.46" y2="78.74" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="4"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="RC4"/>
<wire x1="124.46" y1="73.66" x2="121.92" y2="73.66" width="0.1524" layer="91"/>
<wire x1="121.92" y1="73.66" x2="121.92" y2="35.56" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="3"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="RC3"/>
<wire x1="124.46" y1="68.58" x2="124.46" y2="35.56" width="0.1524" layer="91"/>
<wire x1="124.46" y1="35.56" x2="127" y2="35.56" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="RC2"/>
<wire x1="132.08" y1="58.42" x2="165.1" y2="58.42" width="0.1524" layer="91"/>
<wire x1="165.1" y1="58.42" x2="165.1" y2="68.58" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="1"/>
<wire x1="132.08" y1="35.56" x2="132.08" y2="58.42" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
