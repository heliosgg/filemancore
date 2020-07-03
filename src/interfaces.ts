interface FileInfo {
  /** The full path to this directory item. */
  path: string;

  /** The display name of the directory item. */
  name: string;

  /** The size (in bytes) of the item. */
  size: number;

  /** The last time the directory item was modified. */
  lastModified?: Date;

  /** The date time the directory item was created. */
  created?: Date;

  attributes: {
    /* Whether item has hidden attribute. */
    hidden: boolean;

    /* Whether item is a directory */
    directory: boolean;

    /* Whether item has readonly attribute */
    readonly: boolean;

    /* Whether item has system attribute */
    system: boolean;
  };
}

export { FileInfo };
