<div class="basket-items full-list">
  <form action="<?php print url('basket/update', array('absolue' => true)); ?>" method="POST">
    <?php foreach($basket->get_objects() as $basket_item): ?>
      <?php print theme('basket_item__'. $basket_item->basket_object_type, array('item' => $basket_item)); ?>
    <?php endforeach; ?>
    <button type="submit" class="button invert">
      <span>
        <span>
          <?php print t('Update basket'); ?>
        </span>
      </span>
    </button>
  </form>
</div>

