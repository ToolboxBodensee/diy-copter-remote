

$fn=36;
Text="1N4007";
Size=1;
Solder=1;

module Diode(Text,Size,Solder)
  {
  if(Size==0)
  translate([0,0,2.75])
    {
    color("Black")
      translate([0,0,0])
        rotate([0,90,0])
          cylinder(r=1.45,h=2.54*3,center=true);

    color("White")
      translate([-2.54*1.49,0,0])
        rotate([0,90,0])
          cylinder(r=1.5,h=1);

    color("Silver")
    translate([0,0,0])
      rotate([0,90,0])
        cylinder(d=0.55,h=2.54*4,center=true);
    color("Silver")
    translate([2.54*2,0,-5])
      rotate([0,0,0])
        cylinder(d=0.55,h=10,center=true);
color("Silver")
    translate([-2.54*2,0,-5])
      rotate([0,0,0])
        cylinder(d=0.55,h=10,center=true);
    color("Silver")
    translate([2.54*2,0,0])
      rotate([0,0,0])
        sphere(d=0.55);
color("Silver")
    translate([-2.54*2,0,0])
      rotate([0,0,0])
        sphere(d=0.55);

if(Solder==1)
  {
  color("Silver")
      translate([2.54*2,0,-2.25])
        rotate([0,0,0])
          scale([1,1,1.1])
            sphere(d=2);
  color("Silver")
      translate([-2.54*2,0,-2.25])
        rotate([0,0,0])
          scale([1,1,1.1])
            sphere(d=2);
  }

    color("Gray")
      %translate([0,0,0.5])
        linear_extrude(1)
          text(Text,size=1,valign="center",halign="center");
  }
if(Size==1)
  translate([0,0,3.75])
    {
    color("Black")
      translate([-1,0,0])
        rotate([0,90,0])
          cylinder(r=2,h=2.54*3.5,center=true);

    color("White")
      translate([-2.54*1.99-0.3,0,0])
        rotate([0,90,0])
          cylinder(r=2.05,h=1);

    color("Silver")
    translate([-1.257,0,0])
      rotate([0,90,0])
        cylinder(d=0.55,h=2.54*5,center=true);
    color("Silver")
    translate([2.54*2,0,-5])
      rotate([0,0,0])
        cylinder(d=0.55,h=10,center=true);
color("Silver")
    translate([-2.54*3,0,-5])
      rotate([0,0,0])
        cylinder(d=0.55,h=10,center=true);
    color("Silver")
    translate([2.54*2,0,0])
      rotate([0,0,0])
        sphere(d=0.55);
color("Silver")
    translate([-2.54*3,0,0])
      rotate([0,0,0])
        sphere(d=0.55);

if(Solder==1)
  {
  color("Silver")
      translate([2.54*2,0,-3.25])
        rotate([0,0,0])
          scale([1,1,1.1])
            sphere(d=2);
  color("Silver")
      translate([-2.54*3,0,-3.25])
        rotate([0,0,0])
          scale([1,1,1.1])
            sphere(d=2);
  }

    color("Gray")
      %translate([-1,0,1.05])
        linear_extrude(1)
          text(Text,size=1.5,valign="center",halign="center");
  }

if(Size==2)
  translate([0,0,4.25])
    {
    color("Black")
      translate([0,0,0])
        rotate([0,90,0])
          cylinder(r=2.5,h=2.54*4,center=true);

    color("White")
      translate([-2.54*1.99,0,0])
        rotate([0,90,0])
          cylinder(r=2.55,h=1.5);

    color("Silver")
    translate([0,0,0])
      rotate([0,90,0])
        cylinder(d=0.55,h=2.54*6,center=true);
    color("Silver")
    translate([2.54*3,0,-5])
      rotate([0,0,0])
        cylinder(d=0.55,h=10,center=true);
color("Silver")
    translate([-2.54*3,0,-5])
      rotate([0,0,0])
        cylinder(d=0.55,h=10,center=true);
    color("Silver")
    translate([2.54*3,0,0])
      rotate([0,0,0])
        sphere(d=0.55);
color("Silver")
    translate([-2.54*3,0,0])
      rotate([0,0,0])
        sphere(d=0.55);

if(Solder==1)
  {
  color("Silver")
      translate([2.54*3,0,-3.75])
        rotate([0,0,0])
          scale([1,1,1.1])
            sphere(d=2);
  color("Silver")
      translate([-2.54*3,0,-3.75])
        rotate([0,0,0])
          scale([1,1,1.1])
            sphere(d=2);
  }

    color("Gray")
      %translate([0,0,1.7])
        linear_extrude(1)
          text(Text,size=1.5,valign="center",halign="center");
  }
}

Diode(Text,Size,Solder);

