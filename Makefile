sample_app: sample.c lcr_cmd.c lcr_packetizer.c tcp_client.c
	gcc -Wall $^ -o $@

test_patterns: test_patterns.c lcr_cmd.c lcr_packetizer.c tcp_client.c
	gcc -Wall $^ -o $@

clean:
	rm *.o sample_app test_patterns
