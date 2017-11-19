#include "cdcontainers/common.h"

struct cdc_data_info *cdc_di_shared_ctorc(struct cdc_data_info *other);
void cdc_di_shared_dtor(struct cdc_data_info *info);

#define CDC_HAS_FREE(container) (container->dinfo && container->dinfo->free)
#define CDC_HAS_LT(container) (container->dinfo && container->dinfo->lt)
#define CDC_HAS_GT(container) (container->dinfo && container->dinfo->gt)
#define CDC_HAS_EQ(container) (container->dinfo && container->dinfo->eq)
#define CDC_HAS_SIZE(container) (container->dinfo && container->dinfo->size > 0)
