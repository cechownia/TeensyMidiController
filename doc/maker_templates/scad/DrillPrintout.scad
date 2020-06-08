projection(cut = false);
for (y = [0:1:2])
{
    for (x = [0:1:4])
        {

            translate([15*x,15*y,0])
            circle(d=7, $fn=50);
        }
}
