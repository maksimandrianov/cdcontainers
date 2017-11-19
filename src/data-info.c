#include "data-info.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct cdc_data_info *cdc_di_shared_ctorc(struct cdc_data_info *other)
{
        assert(other != NULL);

        struct cdc_data_info *result;

        if (other->__cnt++ != 0)
                return other;

        result = (struct cdc_data_info *)malloc(sizeof(struct cdc_data_info));
        if (result)
                memcpy(result, other, sizeof(struct cdc_data_info));

        return result;
}

void cdc_di_shared_dtor(struct cdc_data_info *info)
{
        if (info && info->__cnt-- == 0)
                free(info);
}
