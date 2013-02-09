test:
	for t in tests/*; do php "$$t" || break; done

.DELETE_ON_ERROR:
.PHONEY: test

