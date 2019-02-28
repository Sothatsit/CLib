#include "test.h"

//
// Tests
//

/*
 * In README under Features -> String
 */
bool test_String_example() {
    char * name = "CLib";
    int version = 1;
    char * url = "https://github.com/Sothatsit/CLib";

    {
        // Create a String from a format
        String contrived = str_format("name: %s, version: %i, url: %s", name, version, url);

        // Print out each key value pair in contrived
        String keyValue;
        String remaining = contrived;
        while(str_isValid(remaining)) {
            keyValue = str_splitAtC(&remaining, ", ");

            // Print out the result
            char * keyValue_nullTerminated = str_c_destroy(&keyValue);
            {
                // printf("%s\n", keyValue_nullTerminated);
            }
            free(keyValue_nullTerminated);
        }

        // Destroy the String
        str_destroy(&contrived);
    }

    return true;
}

/*
 * In README under Features -> StringBuilder
 */
bool test_StringBuilder_example() {
    String fruits[] = {
        str_create("Apple"),
        str_create("Banana"),
        str_create("Cumquat"),
        str_create("Dragon fruit"),
        str_create("Eggplant")
    };
    u64 numFruits = 5;

    {
        // Create a Builder to build a comma seperated array String
        Builder builder = builder_create(32);

        // Loop through each fruit in fruits and add it to builder
        for(u64 index = 0; index < numFruits; ++index) {
            String fruit = fruits[index];

            if(index != 0)
                builder_appendC(&builder, ", ");

            builder_appendStr(&builder, fruit);
        }

        // Print out the built String. We'll have to create
        // a temporary null-terminated string for printf.
        char * arrayNullTerminated = str_c(builder_str(builder));
        {
            // printf("Fruits: [%s]\n", arrayNullTerminated);
        }
        free(arrayNullTerminated);

        // Destroy the StringBuilder
        // TODO : builder_c_destroy function like str_c_destroy
        builder_destroy(&builder);
    }

    return true;
}



//
// Run Tests
//

void test_examples(int * failures, int * successes) {
    test(String_example);
    test(StringBuilder_example);
}
