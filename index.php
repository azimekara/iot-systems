<html>

<?php  


if($_SERVER['REQUEST_METHOD']=="POST")
{

    require("AzimeAdaFruitClass.php");    
    $feedname=$_POST["feedname"];
    $feedvalue=$_POST["feedvalue"];
    $keyx="aio_vPLG32hKnjMuVeYEn1zjPp30ZmRI";
    
    $aio = new azimeAdaFruit($keyx);
    $feed = $aio->getFeed($feedname);
  //  $feed->send($feedvalue); 
    
      if ( $feedvalue == 1234)
    {
         $feed->send(4); 
    }
   
    if ($feedvalue == 1 || $feedvalue == 2 || $feedvalue == 3)
    {
         $feed->send($feedvalue); 
    }
    
    echo $feed->get();    
    
}


?>

<script>
    
    fetch("https://io.adafruit.com/api/v2/azimekr/feeds?X-AIO-Key=aio_vPLG32hKnjMuVeYEn1zjPp30ZmRI", {
        method: "GET",
        headers: {
          Accept: "application/json",
          "Content-Type": "application/json",
        },
      }).then((response) => response.json())
        .then((responseJson) => {
            
            for(let x in responseJson){
                
                const node = document.createElement("div");
                const textnode = document.createTextNode(responseJson[x].name+":"+responseJson[x].last_value);
                
                if (responseJson[x].last_value == 1) //ldr 
                {
                    //const textnode = document.createTextNode(responseJson[x].name+":"+responseJson[x].last_value);
                    const textnode = document.createTextNode("Akşam Oldu -> lambayı açmak için:1");
                    node.appendChild(textnode);
                }
                
                if (responseJson[x].last_value > 3) // sıcaklık
                {
                    const textnode = document.createTextNode("Sıcaklık"+":"+responseJson[x].last_value);
                   
                   if(responseJson[x].last_value > 65) 
                   {
                       // const textnode = document.createTextNode("Sıcaklık"+":"+responseJson[x].last_value);
                        const textnode = document.createTextNode("Sıcaklık 65°C'nin üstünde -> fanı açmak için:2");
                        node.appendChild(textnode);
                   }
                }
                
                if (responseJson[x].last_value == 3) // yagmur 
                {
                    //const textnode = document.createTextNode(responseJson[x].name+":"+responseJson[x].last_value);
                    const textnode = document.createTextNode("Yağmur yağıyor -> pencereyi kapatmak için:3");
                    node.appendChild(textnode);
                }
                 
                
                document.getElementById("data").appendChild(node);
               
                
            }
            
            
          
        });

    
    
</script>

<form name="azime" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
<select name="feedname">
<!--<option value="lamba">Lamba</option>
<option value="servokapi">Servo Kapi</option>
<option value="servopencere">Servo Pencere</option>
<option value="fan">Fan</option> -->
<option value="webdengelen">webdengelen</option>
</select>
<input type="text" name="feedvalue" id="feedvalue"><br>
<input type="submit" value="Send Data">

</form>

<div id="data"></div>

</html>
