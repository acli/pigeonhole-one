test:
	for t in tests/*; do php -r 'set_include_path("/usr/local/share/simpletest/test");' "$$t"; done

