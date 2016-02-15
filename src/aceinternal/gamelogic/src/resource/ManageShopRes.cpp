#include "ManageShopRes.h"
#pragma warning(push, 2)
#include "Logger.h"
#pragma warning(pop)

#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>

bool ManageShopRes::loadContent(Document * xml_doc)
{
	if (NULL == xml_doc)
	{
		return false;
	}

	Element * root_ele = xml_doc->get_root();

	if (NULL == root_ele)
	{
		return false;
	}

	bool result = true;
	Elements ele_list = root_ele->get_elements();
	for (Elements::iterator it = ele_list.begin(); it != ele_list.end(); ++it)
	{
		result = loadInfo(*it) && result;
	}

    if (!result) {
        return false;
    }

    for (BOOST_AUTO(it, shop_map.begin()); it != shop_map.end(); ++it) {
        uint32 shop_id = it->first;
        ShopContents& contents = *(it->second);

        ShopInfo& info = shopInfoMapR[shop_id];
        BOOST_FOREACH(ShopGrid& grid, contents) {
            info.push_back(&grid);
        }

        shopInfoMap[shop_id] = &shopInfoMapR[shop_id];
    }

	return result;
}

ShopInfoMap const&
ManageShopRes::getShopInfoMap() {
    return shopInfoMap;
}

bool ManageShopRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

    do {
        enum {
            kPayTypeCurrency    =   1,
            kPayTypePhysical,
        };

        uint32 pay_type;
        ShopGrid v;

        if (!(getAttrValue(element, "group_id", v.shop_id)
              && getAttrValue(element, "item", v.item_id)
              && getAttrValue(element, "num", v.item_num)
              && getAttrValue(element, "type", pay_type)
              && getAttrValue(element, "currency", v.currency)
              && getAttrValue(element, "payitem", v.pay_item)
              && getAttrValue(element, "price", v.pricenum)
			  && getAttrValue(element, "limitnum", v.limitnum)
			  && getAttrValue(element, "limittype", v.limittype)
			  && getAttrValue(element, "vip_price", v.vip_price)
			  && getAttrValue(element, "vip_goldblessme_id", v.vip_goldblessme_id)
			  && getAttrValue(element, "goldblessme_id", v.goldblessme_id)
			  && getAttrValue(element, "talk", v.talk)
			  && getAttrValue(element, "lable_id", v.lable_id))) {
            break;
        }

        if (pay_type == kPayTypeCurrency) {
            if (v.currency == 0 || v.pay_item != 0) {
                DEF_LOG_ERROR("mismatched pay type\n");
                break;
            }
        } else if (pay_type == kPayTypePhysical) {
            if (v.pay_item == 0 || v.currency != 0) {
                DEF_LOG_ERROR("mismatched pay type\n");
                break;
            }
        } else {
            DEF_LOG_ERROR("invalid pay type\n");
            break;
        }

        ShopGrid *grid = new ShopGrid(v);
        shop_map[v.shop_id].push_back(grid);

        result = true;
    } while (0);

	return result;
}
