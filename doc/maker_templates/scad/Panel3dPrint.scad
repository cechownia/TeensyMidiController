linear_extrude(10)difference(){
   square(size = [(15*5) - 1,(15*3) - 1], center =false);
union(){
    translate([7,7,0])
for (y = [0:1:2])
{
    for (x = [0:1:4])
        {

            translate([15*x,15*y,0])
            circle(d=7, $fn=100);
        }
}
}
}
