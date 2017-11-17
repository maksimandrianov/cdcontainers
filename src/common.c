#include "cdcontainers/common.h"

#include <assert.h>

struct cdc_data_info *cdc_data_info_dcopy(struct cdc_data_info *other)
{
        assert(other != NULL);

        struct cdc_data_info *result;

        result = (struct cdc_data_info *)malloc(sizeof(struct cdc_data_info));
        if (result)
                memcpy(result, other, sizeof(struct cdc_data_info));

        return result;
}
