<?php

/**
 * Default basket page
 * @return array
 */
function basket_page() {
  $page = element_info('page');
  $page['_basket_page'] = true;
  $basket = Basket::get_basket();
  $page['basket'] = $basket;

  drupal_set_title(t('Notepad'));

  $build = array(
    '#theme' => 'basket_items',
    '#basket' => $basket,
  );
  $page['basket'] = $build;

  return $page;
}