import subprocess

def run_test(program, args, expected_output):
    """Runs the test and compares the output to the expected output."""
    try:
        result = subprocess.run(
            [program] + args, capture_output=True, text=True
        )
        output = result.stdout.strip()
        success = output == expected_output
        status = "PASS" if success else "FAIL"
        print(f"Test {' '.join(args)}: {status}")
        if not success:
            print(f"  Expected: {expected_output}")
            print(f"  Got: {output}")
        return success
    except Exception as e:
        print(f"Test {' '.join(args)}: ERROR")
        print(f"  Exception: {e}")
        return False

# Path to your philosopher program
program = "./philo"  # Change this to the actual path of your compiled program

# Define tests: [(args, expected_output)]
tests = [
    # Valid inputs
    (["4", "800", "200", "200"], "Parsing successful"),
    (["5", "800", "200", "200", "7"], "Parsing successful"),

    # Invalid inputs
    (["4", "-800", "200", "200"], "Error"),
    (["4", "800", "200"], "Error"),
    (["4", "800", "200", "200", "a"], "Error"),

    # Edge cases
    (["0", "800", "200", "200"], "Error"),
    (["1", "800", "200", "200"], "Parsing successful"),
    (["4", "0", "200", "200"], "Error"),
    (["4", "800", "0", "200"], "Error"),
    (["4", "800", "200", "0"], "Error"),

    # Too many arguments
    (["4", "800", "200", "200", "5", "extra"], "Error"),

	# Additional tests
	# Additional tests
    (["2", "100", "50", "50"], "Parsing successful"),
    (["10", "5000", "1000", "1000"], "Parsing successful"),
    (["4", "800", "200", "200", "-1"], "Error"),
    (["4", "800", "200", "200", "0"], "Parsing successful"),
    (["1", "1", "1", "1"], "Parsing successful"),
    (["2147483647", "800", "200", "200"], "Parsing successful"),  # Max int
    (["4", "800", "200", "200", "2147483648"], "Error"),  # Overflow
    (["2", "10", "10", "10"], "Parsing successful"),
    (["4", "800", "200", "200", "-42"], "Error"),
    (["3", "abc", "200", "200"], "Error"),
    (["NaN", "800", "200", "200"], "Error"),
    (["4", "800", "200", "200", " "], "Error"),
]

# Run tests
all_passed = True
for args, expected in tests:
    if not run_test(program, args, expected):
        all_passed = False

if all_passed:
    print("All tests passed!")
else:
    print("Some tests failed.")