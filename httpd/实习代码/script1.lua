print("WIFI SETTING...")
wifi.setmode(wifi.STATION)
wifi.sta.config("openlab","openlab123")
print("IP IS",wifi.sta.getip())
led1 = 0
led2 = 4
gpio.mode(led1, gpio.OUTPUT)
pwm.setup(led2, 500,501)
gpio.write(led1,gpio.LOW)
srv=net.createServer(net.TCP, 30)
srv:listen(80,function(conn)
    conn:on("receive", function(client,request)
        local buf = ""
        local _, _, method, path, vars = string.find(request, "([A-Z]+) (.+)?(.+) HTTP")

        if(method == nil)then
             _, _, method, path = string.find(request, "([A-Z]+) (.+) HTTP")
        end

        local _GET = {}

        if (vars ~= nil)then
            for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do
                _GET[k] = v
            end
        end

        buf = buf.."<h1> ESP8266 Web Server</h1>";
        buf = buf.."<p>GPIO0 <a href=\"?pin=ON1\">ON1</a> <a href=\"?pin=OFF1\">OFF1</a></p>"
        buf = buf.."<p>GPIO4 <a href=\"?pin=ADD\">ADD</a> <a href=\"?pin=DEL\">DEL</a></p>"
        
        local _on,_off = "",""

        if(_GET.pin == "ON1")then
            gpio.write(led1, gpio.LOW)
        elseif(_GET.pin == "OFF1")then
            gpio.write(led1, gpio.HIGH)
        elseif(_GET.pin == "ADD")then
            local duty = pwm.getduty(led2)
            if(duty > 10) then
                pwm.setduty(led2, duty - 100)
            end
        elseif(_GET.pin == "DEL")then
            local duty = pwm.getduty(led2)
            if(duty < 1000) then
                pwm.setduty(led2, duty + 100)
            end
        end

        client:send(buf)
        client:close()
        collectgarbage()
    end)
end)
