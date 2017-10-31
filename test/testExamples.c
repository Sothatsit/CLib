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
        while(!str_isEmpty(remaining)) {
            keyValue = str_splitAtCString(&remaining, ", ");

            // Print out the result
            // printf("%s\n", str_toCString(keyValue));
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
        // Create a StringBuilder to build a comma seperated array
        StringBuilder builder = strbuilder_create(32);

        // Loop through each fruit in fruits and add it to builder
        for(u64 index = 0; index < numFruits; ++index) {
            String fruit = fruits[index];

            if(index != 0)
                strbuilder_appendCString(&builder, ", ");

            strbuilder_appendString(&builder, fruit);
        }

        // Print out the built String
        // printf("Fruits: [%s]\n", str_toCString(builder.string));

        // Destroy the StringBuilder
        strbuilder_destroy(&builder);
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