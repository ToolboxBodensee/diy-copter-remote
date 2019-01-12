
$fn=36;
Text="10uF";
Capacitor_diameter=2.54*2;
Capacitor_height=13;
Type=0;  //0=Electrolitic   1=Ceramic    2=Poliester
Solder=0;

module Capacitor(Text,Capacitor_diameter,Capacitor_height,Type,Solder)
  {
  if(Type==0)
  translate([1.257,0,2.75])
    {
  
    color("Black")
      translate([0,0,Capacitor_height/2])
        rotate([0,0,0])
          cylinder(r=Capacitor_diameter,h=Capacitor_height,center=true);

    color("Silver")
      translate([0,0,Capacitor_height+0.01])
        rotate([0,0,0])
          cylinder(r=Capacitor_diameter-1,h=0.1,center=true);

    color("Silver")
      translate([0,0,-0.01])
        rotate([0,0,0])
          cylinder(r=Capacitor_diameter-2,h=0.1,center=true);

    color("Silver")
      translate([1.257,0,-4.5])
        rotate([0,0,0])
          cylinder(d=0.55,h=10,center=true);

    color("Silver")
      translate([-1.257,0,-4.5])
        rotate([0,0,0])
          cylinder(d=0.55,h=10,center=true);
  
    color("White")
      translate([-Capacitor_diameter+0.9,0,Capacitor_height/2])
        rotate([0,0,180])
          rotate([90,90,90])
            linear_extrude(1)
              text(Text,size=2,valign="center",halign="center");

    if(Solder==1)
      {
      color("Silver")
          translate([2.54*0.5,0,-2.25])
            rotate([0,0,0])
              scale([1,1,1.1])
                sphere(d=2);
      color("Silver")
          translate([-2.54*0.5,0,-2.25])
            rotate([0,0,0])
              scale([1,1,1.1])
                sphere(d=2);
      }

    color("White")
      %translate([0.15,0,0])
        intersection()
          {
          translate([Capacitor_diameter/2,0,0])
            rotate([0,0,0])
              cube([Capacitor_diameter,Capacitor_diameter,Capacitor_height*3],center=true);
          color("Black")
            translate([0,0,Capacitor_height/2+0.02])
              rotate([0,0,0])
                cylinder(r=Capacitor_diameter-0.1,h=Capacitor_height,center=true);
         }
      }
  
if(Type==1)
  {
  translate([0,0,2.75])
  {
  color("Sienna")
  hull()
    {
    color("Black")
      translate([0,0,Capacitor_height/2])
        rotate([90,90,0])
          cylinder(r=Capacitor_diameter,h=0.5,center=true);

    difference()
      {
        translate([0,-Capacitor_diameter+1.5,Capacitor_height/2])
          rotate([90,90,0])
            sphere(Capacitor_diameter);

        translate([0,-50,0])
          cube([100,100,100],center=true);
      }

    difference()
      {
        translate([0,Capacitor_diameter-1.5,Capacitor_height/2])
          rotate([90,90,0])
            sphere(Capacitor_diameter);

        translate([0,50,0])
          cube([100,100,100],center=true);
      }
    }

  color("Silver")
    translate([2.54,0,-4.5])
      rotate([0,0,0])
        cylinder(d=0.55,h=20,center=true);

  color("Silver")
    translate([-2.54,0,-4.5])
      rotate([0,0,0])
        cylinder(d=0.55,h=20,center=true);
  
    if(Solder==1)
      {
      color("Silver")
          translate([2.54*1,0,-2.25])
            rotate([0,0,0])
              scale([1,1,1.1])
                sphere(d=2);
      color("Silver")
          translate([-2.54*1,0,-2.25])
            rotate([0,0,0])
              scale([1,1,1.1])
                sphere(d=2);
      }

  color("White")
    translate([0,-0.5,Capacitor_height/2])
        rotate([90,0,0])
      %linear_extrude(1)
        text(Text,size=2,valign="center",halign="center");
  }
}

if(Type==2)
  {
  translate([0,0,2.75])
  {
  color("DodgerBlue")
  hull()
    {
    color("Black")
      translate([0,0,2])
        rotate([0,0,0])
          cube([2.54*3,2.5,2.54*2],center=true);
    }

  color("Silver")
    translate([2.54,0,-4.5])
      rotate([0,0,0])
        cylinder(d=0.55,h=10,center=true);

  color("Silver")
    translate([-2.54,0,-4.5])
      rotate([0,0,0])
        cylinder(d=0.55,h=10,center=true);
  

    if(Solder==1)
      {
      color("Silver")
          translate([2.54*1,0,-2.25])
            rotate([0,0,0])
              scale([1,1,1.1])
                sphere(d=2);
      color("Silver")
          translate([-2.54*1,0,-2.25])
            rotate([0,0,0])
              scale([1,1,1.1])
                sphere(d=2);
      }

  color("White")
    %translate([0,-0.3,2])
        rotate([90,0,0])
          linear_extrude(1)
            text(Text,size=2,valign="center",halign="center");
  }
}
}
               
Capacitor(Text,Capacitor_diameter,Capacitor_height,Type,Solder);

