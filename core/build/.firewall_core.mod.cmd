savedcmd_/home/tyu/NIS3302_/core/firewall_core.mod := printf '%s\n'   firewall_core.o | awk '!x[$$0]++ { print("/home/tyu/NIS3302_/core/"$$0) }' > /home/tyu/NIS3302_/core/firewall_core.mod
