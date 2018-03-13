all:
	(cd mmt-core; make $(MFLAGS) all)
	(cd mmt-app; make $(MFLAGS) all)

clean:
	(cd mmt-core; make $(MFLAGS) clean)
	(cd mmt-app; make $(MFLAGS) clean)

install:
	(cd mmt-core; make $(MFLAGS) install)
	(cd mmt-app; make $(MFLAGS) install)
	
run:
	java -Dimport=trains.import mmt.app.App
