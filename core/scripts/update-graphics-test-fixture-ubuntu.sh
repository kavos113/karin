export UPDATE_TEST_IMAGES=1
export TEST_PROFILE="ubuntu"

echo "Updating graphics test fixture for profile: $TEST_PROFILE"

TEST_PATH="$(dirname "$(realpath "$0")")/../../build/ubuntu-local-debug/core/test"
DEST_PATH="$(dirname "$(realpath "$0")")/../test/fixtures/test_expect_images"
echo "Test path: $TEST_PATH"
echo "Destination path: $DEST_PATH"

cd "$TEST_PATH" || exit 1
./karin_test
cd - || exit 1

cp -rf "$TEST_PATH/fixtures/test_expect_images/$TEST_PROFILE"/* "$DEST_PATH/$TEST_PROFILE/"