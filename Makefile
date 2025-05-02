all:
	g++ particle_simulator.cpp world.cpp particle.cpp -o particle_simulator -lncurses

clean:
	rm -f particle_simulator
