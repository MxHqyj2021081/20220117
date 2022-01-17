all = cli.o ./module/op.o ./module/socket.o

Cli : $(all)
	cc $(all) -o Cli

.PYTHON:clean
clean:
	rm -rf $(all) core Cli

