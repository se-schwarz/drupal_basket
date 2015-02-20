<?php

if ($item->basket_object_type == 'user'): ?>
  <div class="basket-item user-item basket-list-item">
    <span>
      <a href="<?php print drupal_get_path_alias('user/' . $user->uid); ?>">
        <?php print $item->name; ?>
      </a>
  </span>
  <span>
      <a href="<?php print basket_get_url(array('type' => 'user', 'id' => $item->uid), array('operation' => 'remove', 'goto' => 'basket')); ?>">
        Remove
      </a>
    </span>
  </div>
<?php elseif($item->basket_object_type == 'node'): ?>
  <div class="basket-item node-item basket-list-item">
  <span>
    <a href="<?php print url(drupal_lookup_path('alias', 'node/'.$item->nid), array('absolute' => true)); ?>">
      <?php print $item->title; ?>
    </a>
  </span>
      <span class="item-amount">
    <input type="text"  name="basket-amount[<?php print $item->basket_object->get_identificator() ?>]" value="<?php print $item->basket_object->get_amount(); ?>"/>
  </span>
  <span>
    <a href="<?php print basket_get_url(array('type' => 'node', 'id' => $item->nid), array('operation' => 'remove', 'query'=> array('goto' => 'basket'))); ?>">
      Remove
    </a>
  </span>
  </div>
<?php endif; ?>




