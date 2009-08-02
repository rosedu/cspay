<?php
exec( "python check.py", $output, $return );
foreach ( $output as $file )
    print "$file<br>";
?>